 ///
 /// @file    Thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-05-05 16:28:48
 ///
 
#include "Thread.hpp"
#include <iostream>
using std::cout;
using std::endl;
 
namespace wd
{
	
Thread::~Thread()
{
    cout<<"~Thread()"<<endl;
	if(_isRunning) {
		pthread_detach(_pthid);
	}
}

void Thread::start() //开启线程 ==> pthread_create
{
	/* pthread_create(&_pthid, nullptr, threadFunc, nullptr); */
	pthread_create(&_pthid, nullptr, threadFunc, this);
	_isRunning = true;
}

void Thread::join()  //回收线程
{
	if(_isRunning) {
		pthread_join(_pthid, nullptr);
		_isRunning = false;
	}
}

void * Thread::threadFunc(void * arg)
{
	Thread * pthread = static_cast<Thread*>(arg);
	if(pthread) {
		pthread->_bindfunction();//调用时表现动态多态特性
	}
    return nullptr;
}

}//end of namespace wd
