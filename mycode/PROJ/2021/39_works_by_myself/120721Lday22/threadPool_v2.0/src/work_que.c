#include "../include/head.h"
#include "../include/work_que.h"

int queInit(pQue_t pQue, int capacity){
    pQue->pHead = NULL;
    pQue->pTail = NULL; 

    pQue->queCapacity = capacity;
    pQue->queSize = 0;
    
    pthread_mutex_init(&pQue->que_mutex, NULL);
    return 0;
}

int queInsert(pQue_t pQue, pNode_t pNew){
    if(0 == pQue->queSize){
        pQue->pHead = pNew;
        pQue->pTail = pNew;
    }else{
        pQue->pTail->pNext = pNew;
        pQue->pTail = pNew;
    }
    ++pQue->queSize;

    return 0;
}

int queGet(pQue_t pQue, pNode_t* ppGet){
    if(pQue->queSize){
        //如果队列非空
        *ppGet = pQue->pHead;//出队
        pQue->pHead = pQue->pHead->pNext;
        --pQue->queSize;
        if(0 == pQue->queSize){
            pQue->pTail = NULL;
        }
    }else{
        //队列为空
        return -1;//空则返回-1
    }
    return 0;
}
