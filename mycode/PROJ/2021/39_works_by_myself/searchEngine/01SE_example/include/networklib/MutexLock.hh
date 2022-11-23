#ifndef __WD_MUTEXLOCK_H__
#define __WD_MUTEXLOCK_H__

#include "../../libs/Mylog.hh"
#include "Noncopyable.hh"

#include <pthread.h>
#include <iostream>

namespace wd
{

class MutexLock : Noncopyable
{
public:
    MutexLock() : _isLocking(false)
    {
        if(0 != pthread_mutex_init(&_mutex, NULL)) {
            logError("pthread_mutex_init");
        }
    }

    ~MutexLock()
    {
        if(0 != pthread_mutex_destroy(&_mutex)) {
            logError("pthread_mutex_destroy");
        }
    }

    pthread_mutex_t * getMutexPtr() { return &_mutex; }

    void lock()
    {
        if(0 != pthread_mutex_lock(&_mutex)) {
            logError("pthread_mutex_lock");
        }
        _isLocking = true;
    }

    void unlock()
    {
        if(0 != pthread_mutex_unlock(&_mutex)) {
            logError("pthread_mutex_unlock");
        }
        _isLocking = false;
    }

    bool isLocking() const { return _isLocking; }

private:
    pthread_mutex_t _mutex;
    bool _isLocking = false;
};

class MutexGuard
{
public:
    MutexGuard(MutexLock & mutex)
        :_mutex(mutex)
    { _mutex.lock(); }

    ~MutexGuard()
    { _mutex.unlock(); }

private:
    MutexLock & _mutex;
};

}// end of namespace wd
#endif

