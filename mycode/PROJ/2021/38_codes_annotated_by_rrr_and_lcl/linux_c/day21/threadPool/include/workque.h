#ifndef __WORKQUE_H__
#define __WORKQUE_H__
#include "head.h"

//任务节点的数据结构
typedef struct node{
    int clientFd;
    //新增：command数据结构
    struct node *pNext;
}Node_t, *pNode_t;

//任务队列的数据结构//用链表也要好处，就没有上限
typedef struct{
    int size;
    pNode_t pHead, pTail;
    pthread_mutex_t que_mutex;
    pthread_cond_t que_cond;//为什么放在这，因为【锁和它要守护的资源是放在一起的】
}Que_t, *pQue_t;

int queInit(pQue_t pQue);
int queInsert(pQue_t pQue, pNode_t pNew);
int queGet(pQue_t pQue, pNode_t* pGet);
#endif
