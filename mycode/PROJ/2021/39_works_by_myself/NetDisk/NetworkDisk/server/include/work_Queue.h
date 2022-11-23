#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include"../include/head.h"
#define ARGSLEN_SMALL 1000
#define ARGSLEN_BIG 1004
typedef struct work_Node_s{
    int type;
    int clientFd;
    int *Fd;
    char workArgs[ARGSLEN_BIG];
    struct work_Node_s *pNext; 
}Work_t, *pWork_t;

typedef struct work_Queue_s{
    int workNum;
    pWork_t pHead;
    pWork_t pTail;
    pthread_mutex_t mutex;
}Queue_t, *pQueue_t;

typedef struct thread_Pool_s{
    int flag;
    int threadNum;
    pthread_t *thid;
    pthread_cond_t cond;
    pQueue_t Queue;
}Pool_t, *pPool_t;


int queue_Init(pQueue_t pQue);
int work_Add(pQueue_t pQue, pWork_t pNew);
int work_Get(pQueue_t pQue, pWork_t pNew);
int threadPool_Init(pPool_t pPool, int threadNum, pQueue_t pQue);
void *pthread_Work(void *pPool);
int tcp_Init(char *ip, char *port);
int epoll_Add(int epfd, int sfd);

#endif

