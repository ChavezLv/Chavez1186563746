#include <head.h>

void cleanFunc1(void* p)
{
    printf("cleanFunc1\n");
}
void cleanFunc2(void* p)
{
    printf("cleanFunc2\n");
}

void* threadFunc(void* p)
{
    printf("i am child thread\n");

    //演示线程清理函数栈结构
    pthread_cleanup_push(cleanFunc1, NULL);
    pthread_cleanup_push(cleanFunc2, NULL);

    pthread_cleanup_pop(1);//是类似括号的包括结构，每一个pop弹出的是在它之前离它最近的清理栈（即使不执行也会弹出/不再有效）
    pthread_cleanup_pop(1);

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;

    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");

    pthread_join(thid, NULL);
    return 0;
}

