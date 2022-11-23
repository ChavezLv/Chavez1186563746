 ///
 /// @file    Condition.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:21:41
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
	pthread_cond_wait(&_cond, _mutex.getMutexPtr());
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
