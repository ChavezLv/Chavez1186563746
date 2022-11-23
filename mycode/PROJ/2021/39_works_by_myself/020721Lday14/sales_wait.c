#include <func.h>
/*一个生产者消费者卖票放票的程序，一个生产者，两个消费者，消费者有票就卖票，没有票时等待通知，
 * 生产者运行时如果发现没票了，就放票出来，然后通知消费者卖票。**卖票线程要写退出机制***/
typedef struct Data_s{
    int tickets;
    int exitNum;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
}Data_t,*pData_t;
void* threadCons1(void *p){
    Data_t *data = (pData_t)p;
    while(1){
        pthread_mutex_lock(&data->mutex);
        if(0 == data->tickets){
            if(1 == data->exitNum){
                pthread_mutex_unlock(&data->mutex);
                break;
            }
            pthread_cond_wait(&data->cond,&data->mutex);
            --data->tickets;
            printf("Cons one sales success.\n");
            pthread_mutex_unlock(&data->mutex);
        }else{
            --data->tickets;
            printf("Cons one sales success.\n");
            pthread_mutex_unlock(&data->mutex);
        }
    }
    printf("1\n");
    pthread_exit(NULL);
}
void* threadCons2(void *p){
    Data_t *data = (pData_t)p;
    while(1){
        pthread_mutex_lock(&data->mutex);
        if(0 == data->tickets){
            if(1 == data->exitNum){
                pthread_mutex_unlock(&data->mutex);
                break;
            }
            pthread_cond_wait(&data->cond,&data->mutex);
            --data->tickets;
            printf("Cons two sales success.\n");
            pthread_mutex_unlock(&data->mutex);
        }else{
            --data->tickets;
            printf("Cons two sales success.%d.\n",data->tickets);
            pthread_mutex_unlock(&data->mutex);
        }
    }
    printf("2\n");
    pthread_exit(NULL);
}
void *threadPro(void*p){
    pData_t data = (pData_t)p;
    for(int i = 0; i < 3; ++i){
        while(1){
            if(data->tickets==0){
                pthread_mutex_lock(&data->mutex);
                data->tickets = 10;
                printf("add success.\n");
                pthread_mutex_unlock(&data->mutex);
                pthread_cond_signal(&data->cond);
                break;
            }
        }
    }
    while(1){
        if(data->tickets == 0){
            printf("h\n");
            data->exitNum = 1;
            pthread_cond_broadcast(&data->cond);
            break;
        }
    }
    long val = 1;
    pthread_exit((void*)val);
}
int main(int argc,char*argv[]){
    Data_t data;
    data.exitNum = 0;
    data.tickets = 10;
    int ret;
    
    ret = pthread_mutex_init(&data.mutex,NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");
    ret = pthread_cond_init(&data.cond,NULL);
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
    printf("ret:%ld\n",val);
    if(val >= 0){
        sleep(3);
        pthread_cancel(consumerThid1);
        pthread_cancel(consumerThid2);
    }
    ret = pthread_join(consumerThid1,NULL);
    ret = pthread_join(consumerThid2,NULL);

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond);
    return 0;
}
