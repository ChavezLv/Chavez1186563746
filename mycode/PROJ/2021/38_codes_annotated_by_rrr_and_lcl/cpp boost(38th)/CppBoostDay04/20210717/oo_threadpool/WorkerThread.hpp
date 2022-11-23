 ///
 /// @file    WorkerThread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-17 11:26:00
 ///
 
#ifndef __WD_WORKERTHREAD_HPP
#define __WD_WORKERTHREAD_HPP

#include "Thread.hpp"
#include "Threadpool.hpp"

namespace wd
{

class WorkerThread
: public Thread
{
public:
	WorkerThread(Threadpool & threadpool)
	: _threadpool(threadpool)
	{}

private:
	void run()
	{
		//workerDoTask方法是Threadpool的私有成员函数
		//所以将WorkerThread设置为了Threadpool的友元
		_threadpool.workerDoTask();	
	}
private:
	Threadpool & _threadpool;
};

}//end of namespace wd
 
 
 
#endif
