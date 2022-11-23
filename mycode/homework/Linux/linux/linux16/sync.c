#include<sync.h>
#define N 10
sync_t *sync_creat(){
    sync_t*q=(sync_t*)calloc(1,sizeof(sync_t));
    pthread_cond_init(&q->notfull,NULL);
    pthread_cond_init(&q->notempty,NULL);
    pthread_mutex_init(&q->mutex,NULL);
    return q;
}
void sync_destroy(sync_t* q){
    pthread_cond_destroy(&q->notfull);
    pthread_cond_destroy(&q->notempty);
    pthread_mutex_destroy(&q->mutex);
    free(q);
}
bool sync_isempty(sync_t* q){
    pthread_mutex_lock(&q->mutex);
    bool result=(q->size==0);
    pthread_mutex_unlock(&q->mutex);
    return result;
}
bool sync_isfull(sync_t* q)
{   pthread_mutex_lock(&q->mutex);
    bool result=(q->size==N);
    pthread_mutex_unlock(&q->mutex);
    return result;}
task_t sync_peek(sync_t *q){
   pthread_mutex_lock(&q->mutex);
    while(q->size==0){
        pthread_cond_wait(&q->notempty,&q->mutex);
    }
    task_t task=q->element[q->front];
    pthread_mutex_unlock(&q->mutex);
    return task;
    }
void sync_enque(sync_t* q,task_t elem){
    pthread_mutex_lock(&q->mutex);
    while(q->size==N){
        pthread_cond_wait(&q->notfull,&q->mutex);
    }
    q->element[q->rear]=elem;
    q->rear=(q->rear+1)%N;
    q->size++;
    pthread_cond_signal(&q->notempty);
    pthread_mutex_unlock(&q->mutex);
}
task_t sync_deque(sync_t *q)
{
    pthread_mutex_lock(&q->mutex);
    while(q->size==0){
        pthread_cond_wait(&q->notempty,&q->mutex);
    }
    task_t task=q->element[q->front];
    q->front=(1+q->rear)%N;
    q->size--;
    pthread_cond_signal(&q->notfull);
    pthread_mutex_unlock(&q->mutex);
    return task;
}

