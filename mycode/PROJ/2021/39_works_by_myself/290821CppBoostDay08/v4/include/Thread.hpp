 ///
 /// @file    Thread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-23 15:07:21
 ///
 
#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <functional>
using std::function;

namespace wd
{
//-----------------rrr补充start------------------------
//-----------------rrr补充end------------------------
//用类名来表达语义
//基于对象的写法
class Thread : Noncopyable
{
	using ThreadCallback = function<void()>;
public:
	Thread(ThreadCallback && cb);
	~Thread();

	void start();//开启子线程
	void join();//回收子线程

private:

	//子线程的入口函数,非静态的成员函数
	//第一个参数是隐藏的this指针
	static void * threadFunc(void*);
private:
	pthread_t _pthid;
	bool _isRunning;
	ThreadCallback _cb;
};

}// end of namespace wd
 
 
 
#endif
