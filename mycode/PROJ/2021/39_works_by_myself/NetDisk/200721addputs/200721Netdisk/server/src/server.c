#include"../include/orders.h"

int main(int argc,char*argv[])
{
    ARGS_CHECK(argc, 5)
    //初始化
    int Fd[USERMAX] = {0, 1, 2};

    int threadNum = atoi(argv[3]);
    pQueue_t pOrderQueue = (pQueue_t)calloc(1, sizeof(Queue_t));
    pPool_t pPoolOrder = (pPool_t)calloc(1, sizeof(Pool_t));
    threadPool_Init(pPoolOrder, threadNum, pOrderQueue);
    
    int sfd = tcp_Init(argv[1], argv[2]);
    ERROR_CHECK(sfd, -1, "tcp_init");
    Fd[sfd] = sfd;

    struct epoll_event eves[100];
    int epfd = epoll_create(333);
    epoll_Add(epfd, sfd);

    int ret;
    int readyNum;
    int medFd;//中转用

    printf("\n*******\nserver ready\n*******\n");
    while(1){
        readyNum = epoll_wait(epfd, eves, N, -1);
        for(int i = 0; i < readyNum; ++i){
            if(eves[i].data.fd == sfd){//如果是连接请求
                medFd = accept(sfd, NULL, NULL);
                printf("a client connect\n");
                Fd[medFd] = -1;//-1为未登录用户
                epoll_Add(epfd, medFd);
            }
            else{//如果是命令
                puts("\na message comming");
                recv_Order(eves[i].data.fd, Fd, pPoolOrder);
            }
        }
    }
}

