#include "../head.h"
#include "../include/threadPool.h"
#include "../include/workque.h"

void *threadFunc(void *p)
{
    pQue_t pQue = (pQue_t)p;
    pNode_t pCur;
    int getsuccess = 1;
    while(1){
        pthread_mutex_lock(&pQue->mutex);
        if(0 == pQue->size){//或者说小于等于0
            //等待队列里面有任务//这是睡觉条件
            pthread_cond_wait(&pQue->cond, &pQue->mutex);
        }

        getsuccess = queGet(pQue, &pCur);//临界区
        pthread_mutex_unlock(&pQue->mutex);

        //如果获取成功，就传输文件
        if(0 == getsuccess){
//受到包 处理 
            //（4期）IF所有命令-->处理-->回复
            //

            transFile(pCur->clientFd); //把这里改成完整的逻辑就是3期
            free(pCur);
            pCur = NULL;
            //改复杂逻辑：（3期）这里肯定要包含【完整处理】recv/epoll 
           

            //如果是4.1:（长短命令分离）：这里只有长命令+接过来的是新的tcp链接
        }
    }
}

int threadPoolInit(pThreadPool_t pPool, int threadNum)
{
    pPool->threadNum = threadNum;
    pPool->pthid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    //果然【给指针的原因是因为是动态数组】//我也可以让它静态//那就动态一把子
    queInit(&pPool->Que);
    return 0;
}

int threadPoolStart(pThreadPool_t pPool)
{
    //循环的创建子线程
    for(int i = 0; i < pPool->threadNum; ++i){
        pthread_create(pPool->pthid+i, NULL, threadFunc, &pPool->Que);//然后把地址链接到对应的地址

        /* pthread_create(&pPool->pthid[i], NULL, threadFunc, &pPool->Que); */
    }
    return 0;
}
