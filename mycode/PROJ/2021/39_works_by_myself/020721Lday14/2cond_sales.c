#include <func.h>
/*把上面的程序改变一下，使用两个条件变量，消费者卖完票后通知生产者，然后在1
 * 号条件变量上面等待生产者放票，生产者放票后通知消费者，然后在2号条件变量上面等待消费者把票卖完。
 * */
typedef struct Data_s{
    int tickets;
    int exitN;
    pthread_cond_t cond1;
    pthread_cond_t cond2;
    pthread_mutex_t mutex;
}Data_t,*pData_t;
void cleanFunc(void* p){
    pData_t data = (pData_t)p;
    pthread_mutex_unlock(&data->mutex);
}
void* threadCons1(void *p){
    Data_t *data = (pData_t)p;
    pthread_cleanup_push(cleanFunc,data);
    while(1){
        if(data->tickets>0){
            pthread_mutex_lock(&data->mutex);
            --data->tickets;
            printf("Cons one sales success.\n");
            if(0 == data->tickets){
                pthread_cond_signal(&data->cond2);
                pthread_cond_wait(&data->cond1,&data->mutex);
            }
            pthread_mutex_unlock(&data->mutex);
        }else if(data->exitN && 0 == data->tickets){
            break;
        }else{
            pthread_cond_signal(&data->cond2);
            pthread_cond_wait(&data->cond1,&data->mutex);
        }
    }
    printf("1\n");
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
void* threadCons2(void *p){
    Data_t *data = (pData_t)p;
    pthread_cleanup_push(cleanFunc,data);
    while(1){
        if(data->tickets>0){
            pthread_mutex_lock(&data->mutex);
            --data->tickets;
            printf("Cons two sales success.\n");
            if(0 == data->tickets){
                if(data->exitN){
                    break;
                }
                pthread_cond_signal(&data->cond2);
                pthread_cond_wait(&data->cond1,&data->mutex);
            }
            pthread_mutex_unlock(&data->mutex);
        }else{
            if(data->exitN && 0 == data->tickets){
                break;
            }
            pthread_cond_signal(&data->cond2);
            pthread_cond_wait(&data->cond1,&data->mutex);
        }
    }
    printf("2\n");
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
void *threadPro(void*p){
    pData_t data = (pData_t)p;
    for(int i = 0; i < 30; ++i){
        pthread_mutex_lock(&data->mutex);
        if(data->tickets >= 0){
            pthread_cond_wait(&data->cond2,&data->mutex);
        }
        data->tickets = 10;
        printf("add success.\n");
        pthread_mutex_unlock(&data->mutex);
        pthread_cond_signal(&data->cond1);
    }
    while(1){
        if(data->tickets == 0){
            data->exitN = 1;
            break;
        }
    } 
    printf("3\n");
    pthread_exit(NULL);
}
int main(int argc,char*argv[]){
    Data_t data;
    data.exitN = 0;
    data.tickets = 10;
    int ret;
    
    ret = pthread_mutex_init(&data.mutex,NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
    ret = pthread_cond_init(&data.cond1,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_cond_init");
    ret = pthread_cond_init(&data.cond2,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_cond_init");
    
    pthread_t consumerThid1,consumerThid2,prodThid;
    ret = pthread_create(&consumerThid1,NULL,threadCons1,&data);
    THREAD_ERROR_CHECK(ret , "pthread_create");

    ret = pthread_create(&consumerThid2,NULL,threadCons2,&data);
    THREAD_ERROR_CHECK(ret , "pthread_create");

    ret = pthread_create(&prodThid,NULL,threadPro,&data);
    THREAD_ERROR_CHECK(ret,"pthread_create");

    long val = 0;
    ret = pthread_join(prodThid,(void**)&val);
    if(0 <= val){
        pthread_cancel(consumerThid1);
        pthread_cancel(consumerThid2);
    }
    ret = pthread_join(consumerThid1,NULL);
    ret = pthread_join(consumerThid2,NULL);

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond1);
    pthread_cond_destroy(&data.cond2);
    return 0;
}
