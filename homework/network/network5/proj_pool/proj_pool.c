#include"tcp.h"
#include"process_pool.h"
int expipe[2];
void sigfunc(int signum){
    char  buff[]="son,you should exit";
    send(expipe[1],buff,strlen(buff),0);//epoll_wait监听到有写操作,触发
    printf("the coming of the interrupt signal! ");
}
int main(int argc,char* argv[]){
    signal(SIGPIPE,SIG_IGN);
    signal(SIGUSR1,sigfunc);
    //创建进程池,涉及NUM和子进程结构体指针
    int procNum=atoi(argv[3]);
     process_data_t* pProcessData=(process_data_t*)calloc(procNum,sizeof(process_data_t));
    makeChild(procNum,pProcessData);
    pipe(expipe);
    //服务器初始化,至监听
    int listenfd=tcpInit("192.168.3.119",8888);
    //创建epoll实例
    int epfd=epoll_create1(0);
    epollAddReadEvent(epfd,listenfd);
    epollAddReadEvent(epfd,expipe[0]);//监听读端
    for (int i=0;i<procNum;i++){
        epollAddReadEvent(epfd,pProcessData[i].pipefd);
    }
    struct epoll_event *evtList=(struct epoll_event*)calloc(MAXCONN,sizeof(struct epoll_event));
    int nready=0;
    while(1){
        nready=epoll_wait(epfd,evtList,MAXCONN,-1);
        if(nready==0){
            printf("time out\n");
        }else if(nready==-1){
            perror("epoll_wiat");
        }else if(nready==-1&&errno==EINTR){
            continue;
        }else{
            for(int i=0;i<nready;i++){
                 int fd=evtList->data.fd;
                 if(fd==listenfd){
                    int peerfd=accept(listenfd,NULL,NULL);
                    //分配给子进程
                    for(int i=0;i<procNum;i++){
                        if(pProcessData->busy==CP_FREE){
                            sendFd(pProcessData->pipefd,peerfd,EXIST_T);
                            pProcessData[i].busy=CP_BUSY;
                            break;
                        }
                    }
                 }else if(fd==expipe[0]){
                            char buff[50]={0};
                            recv(expipe[0],buff,sizeof(buff),0);
                            puts(buff);
                            for(int i=0;i<procNum;i++){
                                sendFd(pProcessData->pipefd,0,EXIT_T);//第二个参数可以为零
                            }
                            for(int i=0;i<procNum;i++){
                                wait(NULL);
                            }
                            printf("the processpoll has release!");
                            exit(0);//退出服务器进程
                 }else{
                            for(int i=0;i<procNum;i++){
                                if(pProcessData[i].pipefd==fd){
                                    pProcessData[i].busy=CP_FREE;
                                    printf("Child process[%d] if idle\n ",pProcessData[i].pid);
                                    break;
                                }
                            }
                            
                 }

            }
        }
    }
}
