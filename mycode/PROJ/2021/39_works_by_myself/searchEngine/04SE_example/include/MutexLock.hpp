#ifndef __MUTEXLOCK_HPP__
#define __MUTEXLOCK_HPP__

#include "NonCopyable.hpp"

#include <pthread.h>

namespace spellCorrect
{

class MutexLock
    :NonCopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();
    bool isLocking() const;
    pthread_mutex_t* getMutexLockPtr();
private:
    pthread_mutex_t _mutex;
    bool _isLocking;
};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock& mutex);
    ~MutexLockGuard();
private:
    MutexLock& _mutex;
};

}//end of namespace spellCorrect

#endif
