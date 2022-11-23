 ///
 /// @file    Thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-23 15:10:59
 ///
 
//对于实现文件，第一个头文件必须要是自己
//相关类型的头文件
#include <stdio.h>
#include <errno.h>
#include "Thread.hpp"

namespace wd
{

//Thread(const ThreadCallback &);

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
	//需要一个线程入口函数
	if(pthread_create(&_pthid, nullptr,  threadFunc, this) != 0) {
		perror("pthread_create");	
		return;
	}
	_isRunning = true;
}

void * Thread::threadFunc(void * arg)
{//子线程的入口函数
 //在这里要调用run方法
	 Thread * thread = static_cast<Thread*>(arg);
	 if(thread) {
		thread->_cb();
	 }
	 return nullptr;
}

void Thread::join()
{
	if(_isRunning) {
		//pthread_join是一个阻塞式函数
		pthread_join(_pthid, nullptr);

		_isRunning = false;
	}	
}

}//end of namespace wd

