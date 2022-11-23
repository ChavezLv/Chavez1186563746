#include "../include/head.h"
#include "../include/thread_pool.h"

int threadPoolInit(pThreadPool_t pPool, int threadNum, int capacity){
    queInit(&pPool->que, capacity); 
    
    pthread_cond_init(&pPool->cond, NULL);

    pPool->ptheadID = (pthread_t *)calloc(threadNum, sizeof(pthread_t));

    pPool->threadNum = threadNum;

    pPool->starFlag = '0';

    pPool->endFlag = '0';
    return 0;
}

void* threadFunc(void* p){
    pThreadPool_t pPool = (pThreadPool_t) p;
    pNode_t pGet = NULL;
    int ret = 0;
    while(1){
        pthread_mutex_lock(&pPool->que.que_mutex);
        if(0 == pPool->que.queSize){
            pthread_cond_wait(&pPool->cond, &pPool->que.que_mutex); 
        }
        ret = queGet(&pPool->que, &pGet);
        pthread_mutex_unlock(&pPool->que.que_mutex);

        if(0 == ret){
            transFile(pGet->clientFd);
            //传输结束,释放堆空间
            free(pGet);
            pGet = NULL;
        }
        //exit 
        PRINT_CHECK(pPool->endFlag);
        if('1' == pPool->endFlag){
            break;
        }
    }
    printf("one thread exit\n");
    pthread_exit(0);
}

int threadPoolStart(pThreadPool_t pPool){
    if('0' == pPool->starFlag){
        pPool->starFlag = '1';
        for(int i = 0; i< pPool->threadNum; ++i){
            int ret = pthread_create(&(pPool->ptheadID[i]), NULL, threadFunc, pPool);
            ERROR_CHECK(ret, -1, "pthread_create");
        }
    }

    return 0;
}
