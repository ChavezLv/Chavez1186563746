#ifndef __threadpool_H__
#define __threadpool_H__
#include"func.h"
#include"taskqueue.h"
#define BUFFSIZE 1000
typedef struct{
    pthread_t *threads;
    int threadNumber;
    task_queue_t queue;
}threadpool_t,*pThreadpool_t;

typedef struct{
    int length;
    char data[BUFFSIZE];
}train_t;

void *threadFunc(void*);
void threadpoolInit(threadpool_t*,int);
void threadpoolDestroy(threadpool_t*);
void threadpoolStart(threadpool_t* pthreadpool);
void threadpoolStop(threadpool_t* pthreadpool);
int transferFile(int peerfd);




#endif
