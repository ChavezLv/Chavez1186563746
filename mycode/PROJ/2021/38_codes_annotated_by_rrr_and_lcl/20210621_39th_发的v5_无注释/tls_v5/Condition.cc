 ///
 /// @file    Condition.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-05-07 14:42:31
 ///
 

#include "Condition.hpp"
#include "MutexLock.hpp"

namespace wd
{
	
Condition::Condition(MutexLock & mutex)
: _mutex(mutex)
{
	pthread_cond_init(&_cond, nullptr);
}
	

Condition::~Condition()
{
	pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
	pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
}

void Condition::notify()
{
	pthread_cond_signal(&_cond);
}
	
void Condition::notifyAll()
{
	pthread_cond_broadcast(&_cond);
}
}//end of namespace wd
