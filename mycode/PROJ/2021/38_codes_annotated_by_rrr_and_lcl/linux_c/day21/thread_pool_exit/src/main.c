#include "../include/head.h"
#include "../include/threadPool.h"

int exitPipe[2];
void sigFunc(int sigNum){
    printf("sig is comming\n");
    //异步拉起同步
    write(exitPipe[1], &sigNum, 4);
}
int main(int argc, char** argv)
{
    pipe(exitPipe);

    //父进程收信号
    if(fork()){
        close(exitPipe[0]);
        signal(SIGUSR1, sigFunc);
        wait(NULL);
        printf("child exit\n");
        exit(0);
    }

    //子进程创建线程池
    close(exitPipe[1]);
    int threadNum = atoi(argv[3]);
    
    threadPool_t pool;
    memset(&pool, 0, sizeof(pool));

    //1.初始化线程池
    threadPoolInit(&pool, threadNum);

    //2.启动线程池
    threadPoolStart(&pool);

    //3.创建tcp监听套接字
    int sfd = 0;
    tcpInit(argv[1], argv[2], &sfd);

    //4.创建epoll
    int epfd = epoll_create(1);

    epollAddFd(sfd, epfd);
    epollAddFd(exitPipe[0], epfd);
    struct epoll_event evs[2];

    int newFd = 0;
    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd, evs, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(evs[i].data.fd == sfd){
                newFd = accept(sfd, NULL, NULL);
                pNode_t pNew = (pNode_t)calloc(1, sizeof(Node_t));
                pthread_mutex_lock(&pool.Que.mutex);
                pNew->clientFd = newFd;
                queInsert(&pool.Que, pNew);
                pthread_cond_signal(&pool.Que.cond);
                pthread_mutex_unlock(&pool.Que.mutex);
            }
            else if (evs[i].data.fd == exitPipe[0]){
                //循环的发送cancel信号，退出子进程
                for(int j = 0; j < threadNum; ++j){
                    pthread_cancel(pool.pthid[j]);
                }
                for(int j = 0; j < threadNum; ++j){
                    pthread_join(pool.pthid[j], NULL);
                }
                exit(0);
            }
        }
    }




    return 0;
}

