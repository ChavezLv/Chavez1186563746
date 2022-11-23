#include <head.h>

void* threadFunc(void* p)
{
    printf("i am child thread = %d\n", *(int*)p);

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid, thid2;
    int ret = 0;
    int num = 2;

    ret = pthread_create(&thid, NULL, threadFunc, &num);//2个线程，是并发的，可能启动速度不够快，就一起执行了
    THREAD_ERROR_CHECK(ret, "pthread_create");

    num += 2;
    ret = pthread_create(&thid2, NULL, threadFunc, &num);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");
    sleep(1);
    return 0;
}

