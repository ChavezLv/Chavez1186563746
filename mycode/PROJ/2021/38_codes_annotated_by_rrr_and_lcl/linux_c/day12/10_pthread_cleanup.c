#include <head.h>

//线程清理函数
void cleanFunc(void* p)
{
    printf("cleanFunc\n");
    free(p);
    p = NULL;
}

void* threadFunc(void* p)
{
    char *p1 = (char*)malloc(20);
    strcpy(p1, "hello");
    pthread_cleanup_push(cleanFunc, p1);
    printf("p1 = %s\n", p1);

    free(p1);
    p1 = NULL;
    printf("free sucess\n");

    //return 不会触发线程清理函数的弹栈
    return 0;
    //pthread_exit()会触发线程清理函数的弹栈
    /* pthread_exit(NULL); */
    pthread_cleanup_pop(0);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    
    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");

    /* ret = pthread_cancel(thid); */
    /* THREAD_ERROR_CHECK(ret, "pthread_cancel"); */

    //线程等待函数
    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "join");
    return 0;
}

