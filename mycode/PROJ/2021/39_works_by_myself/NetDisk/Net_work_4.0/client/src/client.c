#include"../include/orders.h"
int main(int argc,char*argv[])
{
    ARGS_CHECK(argc, 2);
    int ret;
    int sfd = tcp_Init();
    ERROR_CHECK(sfd, -1, "connect");
    
    ret = beginSystem(sfd);
    if(ret == -1){
        return -1;
    }
    system("clear");
    printf("welcome!\n");
    
    int threadNum = atoi(argv[1]);
    pQueue_t pTransQueue = (pQueue_t)calloc(1, sizeof(Queue_t));
    pPool_t pPoolTrans = (pPool_t)calloc(1, sizeof(Pool_t));
    threadPool_Init(pPoolTrans, threadNum, pTransQueue);
    
    int epfd = epoll_create(2333);
    struct epoll_event eves[MAX];
    epoll_Add(epfd, sfd);
    epoll_Add(epfd, STDIN_FILENO);

    int readyNum;
    int flag_printf = 1;

    while(1){
        if(flag_printf == 1){
            printf("$");
            fflush(stdout);
        }
        memset(eves, 0, sizeof(eves));
        readyNum = epoll_wait(epfd, eves, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(eves[i].data.fd == STDIN_FILENO){
                ret = send_Order(sfd);
                if(ret != -1){
                    flag_printf = 0;
                }
            }
            if(eves[i].data.fd == sfd){
                read_Order(sfd, pPoolTrans);
                flag_printf = 1;
            }
        }
    }
}
