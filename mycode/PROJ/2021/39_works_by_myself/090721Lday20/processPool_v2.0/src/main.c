#include "../include/func.h"

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 4);
    int processPoolNum = atoi(argv[3]);
    /* printf("2\n"); */

    //循环创建子进程，记录子进程相关信息
    pProcessPool_t pProcessPool;
    processPoolInit(&pProcessPool,processPoolNum);
    processPoolCreate(pProcessPool, processPoolNum);

    //创建tcp监听套接字，等待客户端的连接
    int sockFd;
    tcpInit(argv[1], argv[2], &sockFd); 

    //创建epoll，把需要监听的描述符添加到epoll当中
    int epollFd = epoll_create(1);
    ERROR_CHECK(epollFd, -1, "epoll_create");
    

    for(int i = 0; i < processPoolNum; ++i){
        epollAdd(epollFd, pProcessPool[i].pipeFd);
    }

    epollAdd(epollFd, sockFd);

    struct epoll_event events[6];
    int newFd = 0;
    int readyNum = 0;
    char buf[64] = { 0 };
    while(1){
        readyNum = epoll_wait(epollFd, events, 6, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            //客户端连接服务器，使用accept函数接收这次连接请求，返回newFd， 交给空闲子进程
            if(events[i].data.fd == sockFd){
                newFd = accept(sockFd, NULL, NULL);
                ERROR_CHECK(newFd, -1, "accept");
                epollAdd(epollFd, newFd);
                for(int j = 0; j < processPoolNum; ++j){
                    if(0 == pProcessPool->flag){
                        sendFd(pProcessPool[j].pipeFd, newFd);
                        pProcessPool->flag = 1;
                        close(newFd);//关闭掉父进程这个无用的fd
                        break;
                    }
                }                
            }
            for(int j = 0; j < processPoolNum; ++j){
                if(events[i].data.fd == pProcessPool[j].pipeFd){
                    bzero(buf,sizeof(buf));
                    read(pProcessPool[j].pipeFd, buf, sizeof(buf) - 1);//把内容读走
                    printf("pProcessPool[%d].pipeFd= %d, buf = %s\n", j, pProcessPool[j].pipeFd, buf);
                    pProcessPool[j].flag = 0;
                    /* sleep(3); */
                    break;
                }
            }//for(int j = 0; j < processPoolNum; ++j)
        }//for(int i = 0; i < readyNum; ++i)
    }//while(1)
    return 0;
}

