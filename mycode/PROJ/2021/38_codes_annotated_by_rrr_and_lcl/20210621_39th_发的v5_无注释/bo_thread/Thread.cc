 ///
 /// @file    Thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-11 16:57:23
 ///
 
#include "Thread.hpp"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 
namespace wd
{

namespace current_thread 
{
__thread const char * name = "wd thread";
}

using ThreadCallback = Thread::ThreadCallback ;

struct ThreadData
{
	string _name;
	ThreadCallback _cb;

	ThreadData(const string & name, ThreadCallback && cb)
	: _name(name)
	, _cb(std::move(cb))
	{}

	void runInThread()
	{
		current_thread::name = _name.c_str();
		if(_cb) _cb();
	}
};


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
	ThreadData * pdata = new ThreadData(_name, std::move(_cb));
	int nret = 0;
	if((nret = pthread_create(&_pthid, nullptr, threadFunc, pdata)) != 0) {
		perror("pthread_create");
		fprintf(stderr, "thread error:%s\n", strerror(nret));
		return;
	}
	_isRunning = true;
}

//子线程的入口
void * Thread::threadFunc(void * arg)
{
	//要执行任务，就必要调用run方法, 此时必须要知道Thread对象
	ThreadData * pdata = static_cast<ThreadData*>(arg);
	if(pdata) {
		pdata->runInThread();
	}
	delete pdata;

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
