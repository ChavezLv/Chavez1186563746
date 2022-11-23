#include <head.h>

int main(int argc, char** argv)
{
    pthread_mutex_t mutex;
    //创建锁属性变量
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    //设置锁的属性为检错锁
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &mutexattr);

    int ret = 0;
    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock1");
    printf("first lock\n");

    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock2");
    printf("second lock\n");

    ret = pthread_mutex_unlock(&mutex);
    THREAD_ERROR_CHECK(ret, "unlock1");
    printf("first unlock \n");

    ret = pthread_mutex_unlock(&mutex);
    THREAD_ERROR_CHECK(ret, "unlock2");
    printf("second unlock\n");

    return 0;
}

