#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "head.h"
#include "workque.h"

typedef struct {
    int len;
    char buf[1000];
}Train_t;
//线程池的数据结构
typedef struct{
    int threadNum;
    pthread_t * pthid;
    Que_t Que;
}threadPool_t, *pThreadPool_t;

int threadPoolInit(pThreadPool_t pPool, int threadNum);
int threadPoolStart(pThreadPool_t pPool);
int transFile(int clienFd);
int tcpInit(char *ip, char *port, int *sockFd);
int epollAddFd(int fd, int epfd);
#endif
