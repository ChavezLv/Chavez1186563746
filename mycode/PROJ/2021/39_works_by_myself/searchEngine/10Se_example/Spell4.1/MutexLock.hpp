 ///
 /// @file    MutexLock.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-04-30 10:15:45
 ///

#ifndef __WD_MUTEXLOCK_H__
#define __WD_MUTEXLOCK_H__

#include "noCopy.hpp"
#include <pthread.h>

#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

//表达对象语义
class MutexLock
:public noCopy
{
public:
	MutexLock()
	: _isLocking(false)
	{
		if(0 != pthread_mutex_init(&_mutex, NULL)) {
			perror("pthread_mutex_init");
		}
	}

	//禁止复制控制
	/* MutexLock(const MutexLock &) =delete; */
	/* MutexLock & operator=(const MutexLock &) =delete; */

	~MutexLock()
	{
		if(0 != pthread_mutex_destroy(&_mutex)) {
			perror("pthread_mutex_init");
		}
	}

	void lock()
	{
		if(0 != pthread_mutex_lock(&_mutex)) {
			perror("pthread_mutex_lock");
			return ;
		}
		_isLocking = true;
	}

	void unlock()
	{
		if(0 != pthread_mutex_unlock(&_mutex)) {
			perror("pthread_mutex_unlock");
			return;
		}
		_isLocking = false;
	}

	pthread_mutex_t * getMutexLockPtr()
	{	return &_mutex;	}

	bool isLocking() const 
	{	return _isLocking;	}

private:
	pthread_mutex_t _mutex;
	bool _isLocking;
};

class MutexLockGuard
{
public:
	explicit
	MutexLockGuard(MutexLock & mutex)
	: _mutex(mutex)
	{	_mutex.lock();	}

	~MutexLockGuard()
	{	_mutex.unlock();	}
private:
	MutexLock & _mutex;
};

}//end of namespace wd
 
#endif
