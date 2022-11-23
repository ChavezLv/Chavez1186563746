#include <head.h>

int main(int argc, char** argv)
{
    //静态方式初始化锁
    /* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
    //动态方式初始化锁
    pthread_mutex_t mutex;
    int ret = pthread_mutex_init(&mutex, NULL);
    THREAD_ERROR_CHECK(ret, "init");

    //加锁
    /* ret = pthread_mutex_lock(&mutex); */
    /* THREAD_ERROR_CHECK(ret, "lock"); */
    /* printf("first lock\n"); */

    ret = pthread_mutex_trylock(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_trylock");
    printf("trylock\n");

    //解锁第一次
    ret = pthread_mutex_unlock(&mutex);
    THREAD_ERROR_CHECK(ret, "unlock");

    ret = pthread_mutex_destroy(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_destroy");
    
    return 0;
}

