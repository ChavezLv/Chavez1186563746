 ///
 /// @file    Thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:46:23
 ///
 
#include "Thread.hpp"

namespace wd
{
Thread::Thread(ThreadCallback && cb)
: _pthid(0)
, _isRunning(false)
, _cb(std::move(cb))
{}

Thread::~Thread()
{
	if(_isRunning) {
		pthread_detach(_pthid);	
		_isRunning = false;
	}
}

void Thread::start()
{
	pthread_create(&_pthid, nullptr, threadFunc, this);

	_isRunning = true;
}

//static成员函数没有this指针
void * Thread::threadFunc(void * arg)
{
	Thread * pthread = static_cast<Thread*>(arg);
	if(pthread) {
		pthread->_cb();
	}
	return nullptr;
}

void Thread::join()
{
	if(_isRunning) {
		pthread_join(_pthid, nullptr);
		_isRunning = false;
	}
}

}//end of namespace wd
