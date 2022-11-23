#include <head.h>

void* threadFunc(void* p)
{
    //子线程自己把自己设置为分离属性
    pthread_detach(pthread_self());
    printf("i am child thread\n");
    sleep(2);
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{

    int ret = 0;
    pthread_t thid;
    /* pthread_attr_t attr; */
    /* pthread_attr_init(&attr); */
    /* pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); */

    ret = pthread_create(&thid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_create");
    
    printf("i am main thread\n");

    sleep(1);
    ret = pthread_join(thid, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_join");
    return 0;
}

