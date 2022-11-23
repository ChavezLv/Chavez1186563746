#include<func.h>
int share=0;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
void* routine1(void* arg){
    pthread_mutex_lock(&mutex1);
    sleep(2);
    pthread_mutex_lock(&mutex2);
    for(int i;i<10000000;share++)
        i++;
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return (void*)NULL;
}
void* routine2(void* arg){
    pthread_mutex_lock(&mutex2);
    sleep(2);
    pthread_mutex_lock(&mutex1);
    for(int i;i<10000000;share++)
        i++;
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return (void*)NULL;
}
//void* routine1(void* arg){
//    block();
//    //sleep(2);
//    return (void*)NULL;
//
//}
//void* routine2(void* arg){
//    block();
//    //sleep(2);
//    return (void*)NULL;
//}
int main(){
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,routine1,NULL);
    pthread_create(&tid2,NULL,routine2,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    printf("share=%d\n",share);
    return 0;
}
