#include"taskqueue.h"
#include<func.h>


void queueInit(task_queue_t* que){
    if(que){
        que->pFront=NULL;
        que->pRear=NULL;
        que->queueSize=0;
        que->exitFlag=0;
        int ret=pthread_mutex_init(&que->mutex,NULL);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_init");

        ret=pthread_cond_init(&que->cond,NULL);
        THREAD_ERROR_CHECK(ret,"pthread_cond_init");

    }
}

void queueDestroy(task_queue_t* que){
    if(que){
        int ret=pthread_mutex_destroy(&que->mutex);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_destroy");
        ret=pthread_cond_destroy(&que->cond);
        THREAD_ERROR_CHECK(ret,"pthread_mutex_destroy");
        
    }
}

int queueIsEmpty(task_queue_t* que){
    return que->queueSize==0;
}

int getTaskSize(task_queue_t* que){
    return que->queueSize;

}

//生产者,在父进程中调用
void taskEnqueue(task_queue_t* que,int peerfd){
    pthread_mutex_lock(&que->mutex);
    task_t *pNewtask=(task_t*)calloc(1,sizeof(task_t));
    pNewtask->peerfd=peerfd;
    pNewtask->pnext=NULL;
    if(getTaskSize(que)==0){
        que->pFront=que->pRear=pNewtask;
    }else{
        que->pRear->pnext=pNewtask;
        que->pRear=pNewtask;
    }
    que->queueSize++;
    pthread_mutex_unlock(&que->mutex);
    pthread_cond_signal(&que->cond);
}

//消费者,子线程中调用
int taskDequeue(task_queue_t* que){
    pthread_mutex_lock(&que->mutex);
    while(!que->exitFlag&&queueIsEmpty(que)){//!的优先级高于&&
        pthread_cond_wait(&que->cond,&que->mutex);
    }

    if(!que->exitFlag){//如果退出标志位为零,表示不退出?
        task_t *pDeleteNode=que->pFront;
        int peerfd=pDeleteNode->peerfd;
        if(getTaskSize(que)>1){
            que->pFront=que->pFront->pnext;
        }else{//当只有一个节点时
            que->pFront=que->pRear=NULL;
        }
        que->queueSize--;
        pthread_mutex_unlock(&que->mutex);
        free(pDeleteNode);
        return peerfd;
    }else{
        pthread_mutex_unlock(&que->mutex);
        return -1;
}}

void queueWakeup(task_queue_t* que){
    que->exitFlag=1;//表示不退出?
    pthread_cond_broadcast(&que->cond);
}
