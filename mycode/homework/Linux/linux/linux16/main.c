#include<sync.h>
sync_t*q;
unsigned count=1;
pthread_mutex_t task_mutex=PTHREAD_MUTEX_INITIALIZER;
unsigned gettaskid(){
    pthread_mutex_lock(&task_mutex);
    unsigned result=++count;
    pthread_mutex_unlock(&task_mutex);
    return result;
}
void *produce(void* arg){
    while(1){
        task_t task;
        task.id=gettaskid();
        sync_enque(q,task);
        printf(">>producer %ld:generate task %u\n",
               (long)arg,
               task.id
               );
    }
}
void* comsumer(void* arg){
    while(1){
        task_t task=sync_deque(q);
        printf(">>consumer%ld:execute task %u\n",(long)arg,task.id);
    }
}
int main(void){
    q=sync_creat();
    pthread_t t1,t2,t3,t4;
    pthread_create(&t1,NULL,produce,(void*)1);
    pthread_create(&t2,NULL,produce,(void*)2);
    pthread_create(&t3,NULL,comsumer,(void*)3);
    pthread_create(&t4,NULL,comsumer,(void*)4);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    return 0;
}
