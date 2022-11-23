#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include"head.h"
typedef struct work_Node_s{
    int clientFd;
    struct work_Node_s *pNext; 
}Work_t, *pWork_t;

typedef struct work_Queue_s{
    int flag;
    int workNum;
    pWork_t pHead;
    pWork_t pTail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}Queue_t, *pQueue_t;

int work_Add_send(pQueue_t sendQueue, int clientFd);
int work_Get_send(pQueue_t sendQueue, int *clientFd);
int send_File(int clientFd);

#endif

