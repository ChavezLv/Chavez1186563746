 ///
 /// @file    Thread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:44:17
 ///
 
#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include <pthread.h>


namespace wd
{

class Thread
{
public:
	Thread();
	virtual ~Thread();

	void start();//开启子线程
	void join();

	//冗余: 优化方式就是设置一个noncopyable类
	Thread(const Thread&) = delete;
	Thread& operator= (const Thread&) = delete;

private:
	virtual void run() = 0;//代表子线程要执行的任务
	static void * threadFunc(void * arg);

private:
	pthread_t _pthid;
	bool _isRunning;
};

}//end of namespace wd
 
 
 
#endif
