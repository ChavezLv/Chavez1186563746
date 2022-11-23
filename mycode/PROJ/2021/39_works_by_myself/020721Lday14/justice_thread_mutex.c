#include <func.h>
/*访问一些资源时要先加锁，加锁成功的线程代表抢到了资源，那么多个线程竞争
 * 一把锁时，锁能不能保证公平性，能不能保证资源是平均分配给多个线程的？
 * **设计一个程序验证一下锁的公平性。
 * 沿用之前add时候的程序,修改一下for循环即可***/
#define N 10000000
typedef struct Data_s{
    int num;
    int cnt1;
    int cnt2;
    pthread_mutex_t mutex;
}Data_t,*pData_t;
void* threadAdd1(void *p){
    Data_t *data = (pData_t)p;
    //如果先判断再加锁,有可能两个线程同时进入循环,而且
    //可以此时正处在边界条件,所以可能加100会变成101的情况
    /* while(data->num < N*2){ */
    /*     pthread_mutex_lock(&data->mutex); */
    /*     ++data->num; */
    /*     ++data->cnt1; */
    /*     pthread_mutex_unlock(&data->mutex); */
    /* } */
    while(1){
        pthread_mutex_lock(&data->mutex);
        if(data->num < N*2){
            ++data->num;
            ++data->cnt1;
            pthread_mutex_unlock(&data->mutex);
        }else{
            pthread_mutex_unlock(&data->mutex);
            break;
        }
    }
    pthread_exit(NULL);
}
void* threadAdd2(void *p){
    Data_t *data = (pData_t)p;
    while(1){
        pthread_mutex_lock(&data->mutex);
        if(data->num < N*2){
            ++data->num;
            ++data->cnt2;
            pthread_mutex_unlock(&data->mutex);
        }else{
            pthread_mutex_unlock(&data->mutex);
            break;
        }
    }
    pthread_exit(NULL);
}
int main(int argc,char*argv[]){
    Data_t data;
    data.num = 0;
    data.cnt1 = 0;
    data.cnt2 = 0;
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
    printf("%d times ,Respectively assigned: %d,%d.\n",data.cnt1+data.cnt2,data.cnt1,data.cnt2);    
    pthread_mutex_destroy(&data.mutex);
    return 0;
}

