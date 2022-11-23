#include "process_pool.h"

int main(int argc, char** argv)
{
    //ip,port, processNum;
        
    int processNum = atoi(argv[3]);

    pProcess_data_t pData = (pProcess_data_t)calloc(processNum, sizeof(process_data_t));

    //1.循环创建子进程
    makeChild(processNum, pData);

    //2.创建tcp监听套接字
    int sfd = 0;
    tcpInit(argv[1], argv[2], &sfd);

    //3.创建epoll
    int epfd = epoll_create(1);
    epollAddFd(sfd, epfd);

    for(int i = 0; i < processNum; ++i){
        epollAddFd(pData[i].pipefd, epfd);
    }

    struct epoll_event evs[2];

    int newFd = 0;
    int readyNum = 0;

    while(1){
        readyNum = epoll_wait(epfd, evs, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            //如果是sfd就绪，父进程就接收这次连接，返回的newfd交给一个空闲的子进程
            if(evs[i].data.fd == sfd){
                newFd = accept(sfd, NULL, NULL);
                for(int j = 0; j < processNum; ++j){
                    if(0 == pData[j].flag){
                        sendFd(pData[j].pipefd, newFd);
                        //把任务交给非忙碌的子进程，并且把它的标志位置为1，表示忙碌
                        pData[j].flag = 1;
                        printf("child %d is busy\n", pData[j].pid);
                        break;
                    }
                }
                close(newFd);
            }
            else{
                for(int j = 0; j < processNum; ++j){
                    if(pData[j].pipefd == evs[i].data.fd){

                        //从管道中读数据，把数据读出来
                        char buf[4] = {0};
                        read(pData[j].pipefd, buf, sizeof(buf));
                        //把子进程的状态置为0，表示子进程忙完了
                        pData[j].flag = 0;
                        printf("child %d is not busy\n", pData[j].pid);

                        break;
                    }
                }
            }
        }
    }


    return 0;
}

