#include <head.h>

void* threadFunc(void* p)
{
    pthread_mutex_t *lock = (pthread_mutex_t*)p;
    pthread_mutex_unlock(lock);
    printf("child unlock\n");
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    //静态方式初始化锁
    /* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
    //动态方式初始化锁
    pthread_mutex_t mutex;
    ret = pthread_mutex_init(&mutex, NULL);
    THREAD_ERROR_CHECK(ret, "init");

    ret = pthread_create(&thid, NULL, threadFunc, &mutex);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    //加锁
    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock");
    
    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");

    ret = pthread_mutex_destroy(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_destroy");
    
    return 0;
}

