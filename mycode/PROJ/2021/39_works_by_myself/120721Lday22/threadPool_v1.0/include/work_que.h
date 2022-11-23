#ifndef __WORKQUE_H__
#define __WORKQUE_H__
#include "head.h"

/*元素结构体,存客户端的socket fd*/
typedef struct Node_s{

    int clientFd;
    struct Node_s *pNext;
}Node_t, *pNode_t;

/*描述队列的结构体,存了队列大小, 队列头尾, 和线程共用的cond, 操作队列的mutex*/
typedef struct Que_s{
    pNode_t pHead,pTail;
    int  queCapacity; //队列最大长度
    int queSize;
    /* pthread_cond_t cond; */
    pthread_mutex_t que_mutex;
}Que_t, *pQue_t;

/* int queInit(pQue_t pQue);// 初始化队列 */
int queInit(pQue_t pQue, int capacity);
int queInsert(pQue_t pQue, pNode_t pNew);
int queGet(pQue_t pQue, pNode_t* ppGet);//传入传出, 二级指针

#endif
