#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "MutexLock.h"
class MutexLock;//类的前向声明

class Condition
{
public:
    Condition(MutexLock &mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();

private:
    pthread_cond_t _cond;
    MutexLock &_Mutex;

};

#endif
