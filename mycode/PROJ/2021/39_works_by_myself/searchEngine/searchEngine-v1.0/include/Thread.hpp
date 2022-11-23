/*================================================================
*   
*   @file       ：Thread.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <functional>

#include <string>
using std::string;

using std::function;

namespace wd
{

namespace current_thread{
//这个【线程局部存储】就是【每个线程内的全局变量】
//每块线程可以带一小块独立的信息
extern __thread const char *name;

};


//用类名来表达语义
//基于对象的写法
class Thread : Noncopyable
{
public:
	using ThreadCallback = function<void()>;

	Thread(ThreadCallback && cb, const string &name = "wd thread");
	~Thread();

	void start();//开启子线程
	void join();//回收子线程

private:

	//子线程的入口函数,非静态的成员函数
	//第一个参数是隐藏的this指针
	static void * threadFunc(void*);
private:
	pthread_t _pthid;
    string _name;
	bool _isRunning;
	ThreadCallback _cb;
};

}// end of namespace wd
 
 
 
#endif
