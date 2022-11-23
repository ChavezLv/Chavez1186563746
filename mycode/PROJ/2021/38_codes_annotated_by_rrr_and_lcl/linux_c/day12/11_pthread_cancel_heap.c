#include <head.h>

void* threadFunc(void* p)
{
    char *p1 = (char*)malloc(20);
    strcpy(p1, "hello");
    printf("p1 = %s\n", p1);

    free(p1);
    p1 = NULL;
    printf("free sucess\n");

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    
    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");

    //线程取消函数
    ret = pthread_cancel(thid);
    THREAD_ERROR_CHECK(ret, "pthread_cancel");

    //线程等待函数
    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "join");
    return 0;
}

