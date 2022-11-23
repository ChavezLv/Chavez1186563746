#include <head.h>
//主线程抢占时间片测试
void* threadFunc(void* p)
{
    printf("i am child thread\n");
    //子线程: 接过主线程抢占的时间片->把内容放到缓冲区准备输入到屏幕->被打断->第二次把内容放到缓冲区
    //所以子线程：打印1行、0行、2行的可能性都是存在的
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thid;
    int ret = 0;

    ret = pthread_create(&thid, NULL, threadFunc, NULL);//创建线程//线程函数压栈
    THREAD_ERROR_CHECK(ret, "pthread_create");

    printf("i am main thread\n");
    usleep(1);
    //主线程：这个短暂的沉睡1s会完成：睡觉 ->让出时间片给子线程 -> 马上醒来 -> 抢占时间片(子线程的事没做完)-> 造成子线程重入printf的一些问题
    return 0;
}

