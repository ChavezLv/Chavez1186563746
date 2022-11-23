#include <head.h>

void* threadFunc(void *p)
{
    int retVal = *(int*)p;
    printf("i am child thread val = %d\n", retVal);
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    int val = 2;
    //向子线程里传递整型值
    ret = pthread_create(&thid, NULL, threadFunc, &val); //用值传递的放置强行利用地址传递参数
    THREAD_ERROR_CHECK(ret, "pthread_create");

    sleep(1);
    return 0;
}

