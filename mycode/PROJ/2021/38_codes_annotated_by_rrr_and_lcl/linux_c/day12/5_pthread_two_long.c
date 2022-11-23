#include <head.h>

void* threadFunc(void* p)
{
    printf("i am child thread = %ld\n", (long)p);//用long类型直接传数字【值传递】

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid, thid2;
    int ret = 0;
    long num = 2;

    //传递long类型，是因为指针就是8个字节，所以我们可以传递
    ret = pthread_create(&thid, NULL, threadFunc, (void*)num);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    num += 2;
    ret = pthread_create(&thid2, NULL, threadFunc, (void*)num);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");
    sleep(1);
    return 0;
}

