#include<func.h>
int count=0;
pthread_t tid1,tid2;
void *route1(void *arg){
    tid1=pthread_self();
    while(++count<1000)
        ;
    printf("here is route1!my tid is %lu",tid1);
    return NULL;
}
void *route2(void* arg){
    while(++count<1000)
        ;
    tid1=pthread_self();
    printf("here is route1!my tid is %lu",tid2);
    return NULL;
}
int main(void){
    int ret1=pthread_create(&tid1,NULL,route1,NULL);
    int ret2=pthread_create(&tid2,NULL,route2,NULL);
    sleep(2);
    return 0;
}
