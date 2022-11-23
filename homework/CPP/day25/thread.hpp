#ifndef __thread_H__
#define __thread_H__
#include<pthread.h>
/*thread:
 * create
 * routine_func
 * join
 * exit*/
class Thread{
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
