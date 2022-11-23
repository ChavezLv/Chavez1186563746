#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include <pthread.h>

class MutexLock
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();

    pthread_mutex_t *getMutexLockPtr()
    {
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;

};

/*RAII技术*/
class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }
    
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
public:
    MutexLock &_mutex;
};

#endif
