 ///
 /// @file    MutexLock.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-05-07 14:24:14
 ///
 
#ifndef __WD_MUTEXLOCK_H__
#define __WD_MUTEXLOCK_H__
#include "Noncopyable.hpp"


#include <pthread.h>


namespace wd
{

class MutexLock : Noncopyable
{
public:
	MutexLock();
	~MutexLock();

	void lock();
	void unlock();

	pthread_mutex_t * getMutexLockPtr() {	return &_mutex;	}

private:
	pthread_mutex_t _mutex;//这个类的本质是这把锁
};

class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock & mutex)
	: _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard() {	_mutex.unlock();	}

private:
	MutexLock & _mutex;//这个类的本质就是加和解这把锁
};

}//end of namespace wd

#endif
