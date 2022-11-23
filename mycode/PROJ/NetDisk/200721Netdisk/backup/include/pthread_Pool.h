#ifndef __PTHREAD_POOL_H__
#define __PTHREAD_POOL_H__
#include"head.h"
#include"../include/work_Queue.h"
typedef struct thread_Pool_s{
    int threadNum;
    pthread_t *thid;
    pQueue_t Queue;
}Pool_t, *pPool_t;

void* pthread_Func(void *Queue);
#endif
