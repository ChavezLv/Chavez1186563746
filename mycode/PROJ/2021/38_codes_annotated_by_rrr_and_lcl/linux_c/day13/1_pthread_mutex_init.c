#include <head.h>

int main(int argc, char** argv)
{
    //静态方式初始化锁
    /* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
    //动态方式初始化锁
    pthread_mutex_t mutex;
    int ret = pthread_mutex_init(&mutex, NULL);
    THREAD_ERROR_CHECK(ret, "init");
    
    return 0;
}

