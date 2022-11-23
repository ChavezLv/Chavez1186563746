#ifndef __pthread_H__
#define __pthread_H__
#include <pthread.h>

class MutexLock   
{  
public:      
    //...
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t* getMuxPtr();
private:  
    pthread_mutex_t mutex;
};   

class Condition   
{   
public:      
    //...
    Condition(pthread_mutex_t& mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyall();
private:   
     pthread_cond_t _cond;
     pthread_mutex_t* _mutex;
};




#endif
