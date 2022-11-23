#include"../include/head.h"
#include"../include/work_Queue.h"
#include"../include/pthread_Pool.h"
#include"../include/other_func.h"
int fds[2];
void wait_work_over(int signum){
    printf("\nrecycle start\n*******\n");
    write(fds[1], "1", 1);
}

int main(int argc,char*argv[])
{
    ARGS_CHECK(argc, 4)
    pipe(fds);
    if(fork()){
        close(fds[0]);
        signal(10, wait_work_over);
        wait(NULL);
        return 0;
    }
    close(fds[1]);
    pQueue_t sendQueue = (pQueue_t)calloc(1, sizeof(Queue_t));
    int threadNum = atoi(argv[3]);
    pPool_t pPool = (pPool_t)calloc(1, sizeof(Pool_t));
    init_Pool_Queue(pPool, sendQueue, threadNum); 
    
    int sfd = tcp_init(argv[1], argv[2]);
    ERROR_CHECK(sfd, -1, "tcp_init");

    struct epoll_event events, eves[2];
    events.data.fd = sfd;
    events.events = EPOLLIN;
    int epfd = epoll_create(333);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &events);
    events.data.fd = fds[0];
    epoll_ctl(epfd, EPOLL_CTL_ADD, fds[0], &events);

    int readyNum;
    int clientFd;

    printf("\n*******\nserver ready\n*******\n");
    while(1){
        readyNum = epoll_wait(epfd, eves, N, -1);
        for(int i = 0; i < readyNum; ++i){
            if(eves[i].data.fd == sfd){
                clientFd = accept(sfd, NULL, NULL);
                pthread_mutex_lock(&sendQueue->mutex);
                work_Add_send(sendQueue, clientFd);
                pthread_cond_signal(&sendQueue->cond);
                pthread_mutex_unlock(&sendQueue->mutex);
            }
            if(eves[i].data.fd == fds[0]){
                printf("\n*******\nbigin to recycle\n");
                recycle(pPool);
                return 0;
            }
        }
    }
}

