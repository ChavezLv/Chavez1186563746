#include <head.h>

void* threadFunc(void *p)
{
    //申请一块堆空间，作为退出码传递出去
    char *p1 = (char*)malloc(20); //不推荐的做法，要返回值推荐直接利用long把值传出去
    strcpy(p1, "hello");

    pthread_exit(p1);
}
int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    char *retVal = NULL;
    //捕获子线程的退出码
    ret = pthread_join(thid, (void**)&retVal);
    printf("retVal = %s\n", retVal);
    free(retVal);
    retVal = NULL;


    return 0;
}

