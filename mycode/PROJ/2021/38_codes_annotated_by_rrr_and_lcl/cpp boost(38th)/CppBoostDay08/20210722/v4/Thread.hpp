 ///
 /// @file    Thread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:44:17
 ///
 
#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <functional>


namespace wd
{

using ThreadCallback = std::function<void()>;

class Thread : Noncopyable
{
public:
	Thread(ThreadCallback && cb);
	~Thread();

	void start();//开启子线程
	void join();

private:
	static void * threadFunc(void * arg);

private:
	pthread_t _pthid;
	bool _isRunning;
	ThreadCallback _cb;//代表要执行的任务
};

}//end of namespace wd
 
#endif
