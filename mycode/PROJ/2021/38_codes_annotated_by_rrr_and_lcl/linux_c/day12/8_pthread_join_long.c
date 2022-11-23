#include <head.h>

void* threadFunc(void* p)
{
    printf("i am child thread\n");
    long ret = 3;
    pthread_exit((void*)ret);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;

    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");

    //使用long类型接收子线程的退出码
    long retVal = 0;
    ret = pthread_join(thid, (void**)&retVal);//直接用long接住返回值
    THREAD_ERROR_CHECK(ret, "join");

    printf("retVal = %ld\n", retVal);
    return 0;
}

