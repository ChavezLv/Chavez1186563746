 ///
 /// @file    Condition.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-04-30 14:48:25
 ///
 
#include "Condition.hpp"
#include <iostream>
using std::cout;
using std::endl;
 
namespace wd 
{

Condition::Condition(MutexLock & mutex)
: _mutex(mutex)
{
	if(0  != pthread_cond_init(&_cond, NULL)) {
		perror("pthrad_cond_init");
	}
}

Condition::~Condition()
{
	if(0 != pthread_cond_destroy(&_cond)) {
		perror("pthread_cond_destroy");
	}
}

void Condition::wait()
{
	if(0 != pthread_cond_wait(&_cond, _mutex.getMutexLockPtr())) {
		perror("pthread_cond_wait");
	}
}

void Condition::notify()
{
	if(0 != pthread_cond_signal(&_cond)) {
		perror("pthread_cond_signal");
	}
}

void Condition::notifyAll()
{
	if(0 != pthread_cond_broadcast(&_cond)) {
		perror("pthread_cond_broadcast");
	}
}

}// end of namespace wd
