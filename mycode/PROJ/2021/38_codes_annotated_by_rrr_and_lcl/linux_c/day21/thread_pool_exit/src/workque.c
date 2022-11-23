#include "../include/head.h"
#include "../include/workque.h"

int queInit(pQue_t pQue)
{
    pQue->size = 0;
    pQue->pHead = NULL;
    pQue->pTail = NULL;
    pthread_mutex_init(&pQue->mutex, NULL);
    pthread_cond_init(&pQue->cond, NULL);

    return 0;
}

int queInsert(pQue_t pQue, pNode_t pNew)
{
    //如果队列为空，头尾指针都指向这个节点
    if(pQue->pTail == NULL){
        pQue->pHead = pNew;
        pQue->pTail = pNew;
    }
    //如果队列不为空，采用尾插法，当前为指针的next指向新插入的节点；
    //插入完成后，偏移尾指针到最新节点
    else{
        pQue->pTail->pNext = pNew;
        pQue->pTail = pNew;
    }
    pQue->size++;

    return 0;
}

int queGet(pQue_t pQue, pNode_t *pGet)
{
    /* pthread_mutex_lock(&pQue->mutex); */
    if(0 == pQue->size){
        return -1;
    }

    //获取节点，并且偏移头指针，删除头结点
    *pGet = pQue->pHead;
    pQue->pHead = pQue->pHead->pNext;

    pQue->size--;

    if(0 == pQue->size){
        pQue->pTail = NULL;
    }

    /* pthread_mutex_unlock(&pQue->mutex); */

    return 0;
}


