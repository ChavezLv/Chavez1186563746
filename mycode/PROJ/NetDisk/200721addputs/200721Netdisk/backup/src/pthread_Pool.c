#include"../include/work_Queue.h"
#include"../include/pthread_Pool.h"

void* pthread_Func(void *Queue)
{
    pQueue_t sendQueue = (pQueue_t)Queue;
    int clientFd;
    int ret;
    while(1){
        pthread_mutex_lock(&sendQueue->mutex);
        if(sendQueue->workNum == 0){
            pthread_cond_wait(&sendQueue->cond, &sendQueue->mutex);
        }
        ret = work_Get_send(sendQueue, &clientFd);
        pthread_mutex_unlock(&sendQueue->mutex);
        if(ret == 0){
            ret = send_File(clientFd);
            if(ret == -1){
                printf("trans fail\n*******\n");
            }
            close(clientFd);
        }
        if(sendQueue->flag == 1){
            pthread_mutex_unlock(&sendQueue->mutex);
            break;
        }
    }
    pthread_exit(NULL);
}

