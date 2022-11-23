#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include"head.h"
#define ARGSLEN 1000
typedef struct work_Node_s{
    int flag;
    int type;
    int clientFd;
    int fileName;
    struct work_Node_s *pNext; 
}Work_t, *pWork_t;

typedef struct work_Queue_s{
    int workNum;
    pWork_t pHead;
    pWork_t pTail;
    pthread_mutex_t mutex;
}Queue_t, *pQueue_t;

typedef struct thread_Pool_s{
    int threadNum;
    pthread_t *thid;
    pthread_cond_t cond;
    pQueue_t Queue;
}Pool_t, *pPool_t;


int queueInit(pQueue_t pQue);
int queueInsert(pQueue_t pQue, pWork_t pNew);
int threadPoolInit(pPool_t pPool, int threadNum);
int tcpInit(char *ip, char *port);
int epoll_Add(int epfd, int sfd);

#endif

