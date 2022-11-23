#include <head.h>

int main(int argc, char** argv)
{
    pthread_mutex_t mutex;
    int ret = 0;
    ret = pthread_mutex_init(&mutex, NULL);

    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock1");
    printf("first lock\n");

    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "lock2");
    printf("second lock\n");

    ret = pthread_mutex_unlock(&mutex);
    THREAD_ERROR_CHECK(ret, "unlock1");
    ret = pthread_mutex_unlock(&mutex);
    THREAD_ERROR_CHECK(ret, "unlock2");

    ret = pthread_mutex_destroy(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_destroy");
    return 0;
}

