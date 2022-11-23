#include <func.h>
/*主线程定义一个整数num=0，创建2个子线程，通过传参的方式\
 * 把num传给子线程，让两个子线程分别对这个num各加1000万次，每次加1*/
#define N 1000000000
typedef struct Data_s{
    int num;
    pthread_mutex_t mutex;
}Data_t,*pData_t;
void* threadAdd1(void *p){
    Data_t *data = (pData_t)p;
    for(int i = 0;i < N; ++i){
        pthread_mutex_lock(&data->mutex);
        ++data->num;
        pthread_mutex_unlock(&data->mutex);
    }
    pthread_exit(NULL);
}
void* threadAdd2(void *p){
    Data_t *data = (pData_t)p;
    for(int i = 0;i < N; ++i){
        pthread_mutex_lock(&data->mutex);
        ++data->num;
        pthread_mutex_unlock(&data->mutex);
    }
    pthread_exit(NULL);
}
int main(int argc,char*argv[]){
    Data_t data;
    data.num = 0;
    int ret;
    ret = pthread_mutex_init(&data.mutex,NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
    
    pthread_t thid1,thid2;
    ret = pthread_create(&thid1,NULL,threadAdd1,&data);
    THREAD_ERROR_CHECK(ret , "pthread_create");

    ret = pthread_create(&thid2,NULL,threadAdd2,&data);
    THREAD_ERROR_CHECK(ret , "pthread_create");

    ret = pthread_join(thid1,NULL);
    ret = pthread_join(thid2,NULL);
    printf("sum = %d\n",data.num);    
    pthread_mutex_destroy(&data.mutex);
    return 0;
}

