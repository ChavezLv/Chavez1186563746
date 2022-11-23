#include <head.h>

void* threadFunc(void* p)
{
    //while（1）使子线程一直处于循环，无法运行到取消点
    /* while(1); */
    sleep(1);
    printf("i am child thread\n");

    printf("hello main\n");

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    
    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");

    //线程的取消函数
    ret = pthread_cancel(thid);
    THREAD_ERROR_CHECK(ret, "cancel");

    //线程等待函数
    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "join");
    return 0;
}

