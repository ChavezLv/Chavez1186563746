#ifndef __WORK_QUEUE_H__
#define __WORK_QUEUE_H__
#include"../include/head.h"
#define ARGSLEN 1000 

//任务节点
typedef struct work_Node_s{
    int type;//类型
    int clientFd;//客户端套接字
    int *Fd;//存放listenfd的数组
    void* pPool;//线程池
    char workArgs[1024];//用以接收命令(cd,ls..)
    struct work_Node_s *pNext; //下个节点
}Work_t, *pWork_t;

//任务队列
typedef struct work_Queue_s{
    int workNum;//任务队列号数
    pWork_t pHead;
    pWork_t pTail;
}Queue_t, *pQueue_t;


typedef struct thread_Pool_s{
    int flag;//退出标志位
    int threadNum;//线程数量
    pthread_t *thid;//线程号
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pQueue_t Queue;//任务队列
}Pool_t, *pPool_t;



int work_Add(pQueue_t pQue, pWork_t pNew);
int work_Get(pQueue_t pQue, pWork_t *pNew);
int threadPool_Init(pPool_t pPool, int threadNum, pQueue_t pQue);
void *pthread_Work(void *pPool);
int tcp_Init(void);
int epoll_Add(int epfd, int sfd);
#endif

