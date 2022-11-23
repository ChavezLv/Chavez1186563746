#include <head.h>

void* threadFunc(void* p)
{
    printf("i am child thread %s\n", (char *)p);
    strcpy((char*)p, "hellomain");//堆空间是不会随着栈的消逝而释放的，永生了（与此同时，也存在内存泄漏的风险）
    //所以推荐今线程事今线程毕，不要带出函数//非带出不可就用传入传出参数，你自己好在main里控制

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    char *p = (char*)malloc(20);
    strcpy(p, "hellochild");

    //向子线程传递堆空间
    ret = pthread_create(&thid, NULL, threadFunc, p);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");
    sleep(1);

    printf("p = %s\n", p);
    return 0;
}

