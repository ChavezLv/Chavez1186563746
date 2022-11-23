#include"threadpool.h"
void threadpoolInit(threadpool_t* pthreadpool,int threadNum){
   if(pthreadpool){
        pthreadpool->threadNumber=threadNum;
        pthreadpool->threads=(pthread_t*)calloc(threadNum,sizeof(pthread_t));//pthread_t是什么?
        queueInit(&pthreadpool->queue);
    }
}

void threadpoolDestroy(threadpool_t *pthreadpool){
    if(pthreadpool){//?
        free(pthreadpool->threads);
        queueDestroy(&pthreadpool->queue);
    }
}

void threadpoolStart(threadpool_t *pthreadpool){
    for(int i=0;i<pthreadpool->threadNumber;++i){
    }
}

void* threadFunc(void *arg){
    threadpool_t* pthreadpool=(threadpool_t*)arg;
    while(1){
        if(pthreadpool){
            int peerfd=taskDequeue(&pthreadpool->queue);
            if(peerfd>0){
                transferFile(peerfd);
            }else{break;}
        }
    }
}

void threadpoolStop(threadpool_t* pthreadpool){
    while(!queueIsEmpty(&pthreadpool->queue)){
        sleep(1);
    }
    queueWakeup(&pthreadpool->queue);
    for(int i=0;i<pthreadpool->threadNumber;++i){
        pthread_join(pthreadpool->threads[i],NULL);
    }
}


