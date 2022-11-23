 ///
 /// @file    MutexLock.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:15:04
 ///
 
#ifndef __WD_MUTEXLOCK_HPP__
#define __WD_MUTEXLOCK_HPP__

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

	pthread_mutex_t * getMutexPtr() {	return &_mutex;	}

private:
	pthread_mutex_t _mutex;
};

//RAII技术的应用:用对象的生命周期来管理资源
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
	MutexLock & _mutex;

};

}//end of namespace wd
 
 
 
#endif
