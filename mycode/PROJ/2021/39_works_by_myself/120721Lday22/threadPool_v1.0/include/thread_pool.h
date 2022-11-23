#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#include "head.h"
#include "work_que.h"

/*首先记录一个队列,队列里面有一个任务,然后通知等待队列上面的条件
 * 变量cond(也可以放到Que_t结构体里面), 线程id, 可以为数组or 指针 , 线程数, starFlag初始化时为0*/
typedef struct ThreadPool_s{
    Que_t que;
    pthread_cond_t cond;
    pthread_t *ptheadID; //存线程ID的起始地址
    int threadNum;//thread numbers
    char starFlag;//启动标志
    char endFlag;//是否退出的标志
}ThreadPool_t,*pThreadPool_t;


int threadPoolInit(pThreadPool_t pPool, int threadNum, int capacity); 
int threadPoolStart(pThreadPool_t pPool);

#endif
