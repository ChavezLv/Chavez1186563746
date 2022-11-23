#include <head.h>

void* threadFunc(void* p)
{
    printf("i am child thread\n");

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;
    
    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");

    //线程等待函数
    ret = pthread_join(thid, NULL);//join了线程以后，会进行一些资源的清理，长期不join死掉的线程，也会有僵尸线程问题
    THREAD_ERROR_CHECK(ret, "join");
    return 0;
}

