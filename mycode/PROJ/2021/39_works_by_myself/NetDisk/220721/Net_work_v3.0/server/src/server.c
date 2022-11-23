#include"../include/orders.h"

int main(int argc,char*argv[])
{
    ARGS_CHECK(argc, 2)
    //初始化
    int Fd[USERMAX] = {0, 1, 2};

    int threadNum = atoi(argv[1]);
    pQueue_t pOrderQueue = (pQueue_t)calloc(1, sizeof(Queue_t));
    pPool_t pPoolOrder = (pPool_t)calloc(1, sizeof(Pool_t));
    threadPool_Init(pPoolOrder, threadNum, pOrderQueue);
    
    int sfd = tcp_Init();
    ERROR_CHECK(sfd, -1, "tcp_init");
    Fd[sfd] = sfd;
    struct epoll_event eves[100];
    int epfd = epoll_create(333);
    epoll_Add(epfd, sfd);
    int readyNum;
    int medFd;//中转用
    printf("\n$system>> server ready\n*******************\n");

    while(1){
        readyNum = epoll_wait(epfd, eves, N, -1);
        for(int i = 0; i < readyNum; ++i){
            if(eves[i].data.fd == sfd){//如果是连接请求
                medFd = accept(sfd, NULL, NULL);
                printf("\n$system>> a client connect\n");
                Fd[medFd] = -1;//-1为未登录用户
                epoll_Add(epfd, medFd);
            }
            else{//如果是命令
                printf("\n$system>> a message comming\n");
                recv_Order(eves[i].data.fd, Fd, pPoolOrder);
            }
        }
    }
}
