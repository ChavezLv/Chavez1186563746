#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    void start();
    void stop();
private:
    //线程入口函数
    static void *threadFunc(void *arg);
    //线程的操作
    virtual void run() = 0;

private:
    pthread_t _thid;
    bool _isRunning;

};

#endif
