#include "../head.h"
#include "../include/threadPool.h"
#include "../include/workque.h"


void cleanFunc(void *p)
{
    pQue_t pQue = (pQue_t)p;
    
    //解锁
    pthread_mutex_unlock(&pQue->mutex);
}
void *threadFunc(void *p)
{
    pQue_t pQue = (pQue_t)p;
    pNode_t pCur;
    int getsuccess = 1;
    while(1){
        pthread_mutex_lock(&pQue->mutex);
        //使用线程终止清理函数清理锁资源
        pthread_cleanup_push(cleanFunc, pQue);
        if(0 == pQue->size){
            //等待队列里面有任务
            pthread_cond_wait(&pQue->cond, &pQue->mutex);
        }
        getsuccess = queGet(pQue, &pCur);
        pthread_mutex_unlock(&pQue->mutex);
        if(0 == getsuccess){
            transFile(pCur->clientFd);
            free(pCur);
            pCur = NULL;
        }
        pthread_cleanup_pop(1);
    }
    pthread_exit(NULL);
}

int threadPoolInit(pThreadPool_t pPool, int threadNum)
{
    pPool->threadNum = threadNum;
    pPool->pthid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    queInit(&pPool->Que);
    return 0;
}

int threadPoolStart(pThreadPool_t pPool)
{
    //循环的创建子线程
    for(int i = 0; i < pPool->threadNum; ++i){
        pthread_create(pPool->pthid+i, NULL, threadFunc, &pPool->Que);

        /* pthread_create(&pPool->pthid[i], NULL, threadFunc, &pPool->Que); */
    }
    return 0;
}
