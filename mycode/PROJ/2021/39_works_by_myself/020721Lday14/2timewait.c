#include <func.h>
/*把上面的程序改变一下，使用两个条件变量，消费者卖完票后通知生产者，然后在1
 * 号条件变量上面等待生产者放票，生产者放票后通知消费者，然后在2号条件变量上面等待消费者把票卖完。
 * */
#define N 100000
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
    struct timespec spec;
    memset(&spec,0,sizeof(spec));
    int ret ;
    while(1){
    /* printf("1while\n"); */
        spec.tv_sec = time(NULL) + 10;
        ret = pthread_mutex_lock(&data->mutex);
        /* printf("ret2 = %d\n",ret); */
        if(data->tickets>0){
            --data->tickets;
            printf("Cons 1 s.\n");
            if(0 == data->tickets){
                if(data->exitN){
                    pthread_mutex_unlock(&data->mutex);
                    break;
                }
                pthread_cond_signal(&data->cond2);
    printf("1self\n");
                /* pthread_cond_timedwait(&data->cond1,&data->mutex,&spec); */
                pthread_cond_wait(&data->cond1,&data->mutex);
    printf("1after wait\n");
            }
        }else{
            if(data->exitN){
                pthread_mutex_unlock(&data->mutex);
                break;
            }
            pthread_cond_signal(&data->cond2);
    printf("1other\n");
            pthread_cond_wait(&data->cond1,&data->mutex);
            /* pthread_cond_timedwait(&data->cond1,&data->mutex,&spec); */
    printf("1after wait\n");
        }
        pthread_mutex_unlock(&data->mutex);
    }
    printf("2\n");
    pthread_exit(NULL);
}
void* threadCons2(void *p){
    Data_t *data = (pData_t)p;
    struct timespec spec;
    memset(&spec,0,sizeof(spec));
    int ret ;
    while(1){
    /* printf("2while\n"); */
        spec.tv_sec = time(NULL) + 1;
        ret = pthread_mutex_lock(&data->mutex);
        /* printf("ret2 = %d\n",ret); */
        if(data->tickets>0){
            --data->tickets;
            printf("Cons 2 s.\n");
            if(0 == data->tickets){
                if(data->exitN){
                    pthread_mutex_unlock(&data->mutex);
                    break;
                }
                pthread_cond_signal(&data->cond2);
    printf("2self\n");
                pthread_cond_wait(&data->cond1,&data->mutex);
                /* pthread_cond_timedwait(&data->cond1,&data->mutex,&spec); */
    printf("2after wait\n");
            }
        }else{
            if(data->exitN){
                pthread_mutex_unlock(&data->mutex);
                break;
            }
            pthread_cond_signal(&data->cond2);
    printf("2other\n");
            pthread_cond_wait(&data->cond1,&data->mutex);
            /* pthread_cond_timedwait(&data->cond1,&data->mutex,&spec); */
    printf("2after wait\n");
        }
        pthread_mutex_unlock(&data->mutex);
    }
    printf("2\n");
    pthread_exit(NULL);
}
/* void* threadCons1(void *p){ */
/*     Data_t *data = (pData_t)p; */
/*     struct timespec spec; */
/*     memset(&spec,0,sizeof(spec)); */
/*     while(1){ */
/*         spec.tv_sec = time(NULL) + 2; */
/*         if(data->tickets>0){ */
/*             pthread_mutex_lock(&data->mutex); */
/*             --data->tickets; */
/*             printf("Cons one sales success.\n"); */
/*             if(0 == data->tickets){ */
/*                 pthread_cond_signal(&data->cond2); */
/*                 pthread_cond_timedwait(&data->cond1,&data->mutex,&spec); */

/*             } */
/*             pthread_mutex_unlock(&data->mutex); */

/*         }else if(data->exitN && 0 == data->tickets){ */
/*             break; */

/*         }else{ */
/*             pthread_cond_signal(&data->cond2); */
/*             pthread_cond_timedwait(&data->cond1,&data->mutex,&spec); */
/*         } */

/*     } */
/*     printf("1\n"); */
/*     pthread_exit(NULL); */
/* } */
/* void* threadCons2(void *p){ */
/*     Data_t *data = (pData_t)p; */
/*     pthread_cleanup_push(cleanFunc,data); */
/*     while(1){ */
/*     printf("2while\n"); */
/*         if(data->tickets>0){ */
/*             pthread_mutex_lock(&data->mutex); */
/*             --data->tickets; */
/*             printf("Cons two sales success.\n"); */
/*             if(0 == data->tickets){ */
/*                 if(data->exitN){ */
/*                     break; */
/*                 } */
/*                 pthread_cond_signal(&data->cond2); */
/*                 pthread_cond_wait(&data->cond1,&data->mutex); */
/*             } */
/*             pthread_mutex_unlock(&data->mutex); */
/*         }else{ */
/*     printf("2other\n"); */
/*             if(data->exitN && 0 == data->tickets){ */
/*                 break; */
/*             } */
/*             pthread_cond_signal(&data->cond2); */
/*             pthread_cond_wait(&data->cond1,&data->mutex); */
/*         } */
/*     } */
/*     printf("2\n"); */
/*     pthread_cleanup_pop(1); */
/*     pthread_exit(NULL); */
/* } */
void *threadPro(void*p){
    pData_t data = (pData_t)p;
    for(int i = 0; i < N; ++i){
        pthread_mutex_lock(&data->mutex);
        if(data->tickets > 0){
            pthread_cond_wait(&data->cond2,&data->mutex);
        }
        /* data->tickets = 10; */
        data->tickets = 3;
        printf("add success.\n");
        pthread_mutex_unlock(&data->mutex);
        /* pthread_cond_signal(&data->cond1); */
        pthread_cond_broadcast(&data->cond1);
    }
    /* while(1){ */
    /*     if(data->tickets == 0){ */
    /*         data->exitN = 1; */
    /*         break; */
    /*     } */
    /* } */ 
    /* sleep(1); */
    printf("3\n");
    data->exitN = 1;
    pthread_cond_broadcast(&data->cond1);
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
        /* pthread_cancel(consumerThid1); */
        /* pthread_cancel(consumerThid2); */
    }
    ret = pthread_join(consumerThid1,NULL);
    ret = pthread_join(consumerThid2,NULL);

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond1);
    pthread_cond_destroy(&data.cond2);
    return 0;
}
