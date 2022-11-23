 ///
 /// @file    Thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:46:23
 ///
 
#include "Thread.hpp"

namespace wd
{
//只在thread.cc中有一份
namespace current_thread
{
__thread const char * name = "wd thread";
}

Thread::Thread(ThreadCallback && cb, const string & name)
: _pthid(0)
, _name(name)
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
	current_thread::name = pthread->_name.c_str();
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
