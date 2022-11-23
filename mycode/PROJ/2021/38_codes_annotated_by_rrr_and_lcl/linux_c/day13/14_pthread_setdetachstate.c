#include <head.h>

void* threadFunc(void* p)
{
    printf("i am child thread\n");
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{

    int ret = 0;
    pthread_t thid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //设置线程属性为可分离状态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&thid, &attr, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("i am main thread\n");

    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");
    return 0;
}

