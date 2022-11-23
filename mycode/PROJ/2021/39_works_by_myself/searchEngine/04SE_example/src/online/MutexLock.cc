#include "../../include/MutexLock.hpp"

#include <stdio.h>

namespace spellCorrect
{
MutexLock::MutexLock()
    :_isLocking(false)
{
    if(pthread_mutex_init(&_mutex,nullptr))
    {
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock()
{
    if(pthread_mutex_destroy(&_mutex))
    {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock()
{
    if(pthread_mutex_lock(&_mutex))
    {
        perror("pthread_mutex_lock");
        return;
    }
    _isLocking = true;
}

void MutexLock::unlock()
{
    if(pthread_mutex_unlock(&_mutex))
    {
        perror("pthread_mutex_unlock");
        return;
    }
    _isLocking = false;
}

bool MutexLock::isLocking() const
{
    return _isLocking;
}

pthread_mutex_t* MutexLock::getMutexLockPtr()
{
    return &_mutex;
}

MutexLockGuard::MutexLockGuard(MutexLock& mutex)
    :_mutex(mutex)
{
    _mutex.lock();
}

MutexLockGuard::~MutexLockGuard()
{
    _mutex.unlock();
}

}//end of namespace spellCorrect

