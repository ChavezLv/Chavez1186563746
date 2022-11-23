#include"threadpool.h"
#include"tcp.h"
int exitPipe[2];
void sigFunc(int signum){
    printf("signum is coming\n");
    write(exitPipe[1],"a",1);
}

int main(int argc,char* argv[]){
    ARGS_CHECK(argc,4);
    signal(SIGUSR1,sigFunc);
    pipe(exitPipe);
    pid_t ret=fork();
    if(ret){
        wait(NULL);
        exit(0);
    }
    int threadNum=atoi(argv[3]);
    threadpool_t threadpool;

    threadpoolInit(&threadpool,threadNum);
    threadpoolStart(&threadpool);
    int listenfd=tcpInit(argv[1],atoi(argv[2]));
    int epfd=epoll_create1(0);
    ERROR_CHECK(epfd,-1,"epoll_create1");
    epollAddReadEvent(epfd,listenfd);
    epollAddReadEvent(epfd,exitPipe[0]);
    struct epoll_event* pevtList=(struct epoll_event*)calloc(MAXCONN,sizeof(struct epoll_event));
    int nready=0;
    while(1){
        nready=epoll_wait(epfd,pevtList,MAXCONN,5000);
        if(nready==-1&&errno==EINTR){
            continue;
        }else if(nready==-1){
            perror("epoll_wait");
        }else if(nready==0){
            printf("epoll_wait timeout\n");
        }else{
            for(int i = 0; i < nready; ++i) {
				int fd = pevtList[i].data.fd;
				if(fd == listenfd) {
					int peerfd = accept(listenfd, NULL, NULL);
					if(peerfd == -1) {
						perror("accept");
						continue;
					}
                    taskEnqueue(&threadpool.queue,peerfd);
                    printf("peerfd has enter the queue\n");
                }
                if(fd==exitPipe[0]){
                    char ch=0;
                    read(exitPipe[0],&ch,1);
                    printf("threadpool begin exit\n");
                    threadpoolStop(&threadpool);
                    threadpoolDestroy(&threadpool);
                    exit(0);
                }
            }
    close(listenfd);
    close(epfd);
        }
    }
}
