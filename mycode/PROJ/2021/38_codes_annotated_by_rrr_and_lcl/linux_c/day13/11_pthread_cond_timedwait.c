#include <head.h>

typedef struct {
    int num;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}Data_t;

void* threadFunc(void* p)
{
    Data_t *pData = (Data_t*)p;
    struct timespec spec;
    spec.tv_sec = time(NULL) + 3;
    pthread_mutex_lock(&pData->mutex);
    if(0 == pData->num){
        printf("befor wait\n");
        pthread_cond_timedwait(&pData->cond, &pData->mutex, &spec);
        printf("after wait\n");
    }
    pthread_mutex_unlock(&pData->mutex);
    printf("买到酒\n");
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    Data_t data;
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond, NULL);
    data.num = 0;
    ret = pthread_create(&thid, NULL, threadFunc, &data);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    sleep(4);
    pthread_mutex_lock(&data.mutex);
    ret = pthread_cond_signal(&data.cond);
    THREAD_ERROR_CHECK(ret, "pthread_cond_signal");
    printf("有酒了\n");
    pthread_mutex_unlock(&data.mutex);

    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join"); 

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond);
    
    return 0;
}

