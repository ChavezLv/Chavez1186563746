#ifndef __work_Queue_H__
#define __work_Queue_H__
#include<head.h>
#define ARGSLEN 1000
typedef struct work_Node_s{
    int type;
    int clientfd;
    int *Fd;
    void *pPool;
    char workArgs[1024];
    struct work_Node_s* pNext;
}Work_t,*pWork_t;

typedef  struct work_Queue_s{
    int workNum;
    pWork_t pHead;
    pWork_t pTail;
}Queue_t,*pQueue_t;

typedef struct thread_Pool_s{
    int flag;//退出标志位
    int threadNum;
    pthread_t *thrid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pQueue_t Queue;
}Pool_t,*pPool_t;

int work_Add(pQueue_t pQue, pWork_t pNew);
int work_Get(pQueue_t pQue, pWork_t *pNew);
int threadPool_Init(pPool_t pPool, int threadNum, pQueue_t pQue);
void *pthread_Work(void *pPool);
int tcp_Init(void);
int epoll_Add(int epfd, int sfd);
#endif
