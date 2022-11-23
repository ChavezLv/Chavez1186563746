 ///
 /// @file    Thread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-11 16:52:09
 ///
 

/*BO的线程*/

#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <string>
#include <functional>
using std::string;
 
namespace wd
{


/* __thread int idx = 0; *///没有适用性的做法//唯一的修正就在于【不放int】

namespace current_thread 
{
extern __thread const char * name;//每块线程可以带一小块独立的信息
//这个【线程局部存储】就是【每个线程内的全局变量】
}

class Thread : Noncopyable
{
//2-再看接口
public:
	using ThreadCallback = std::function<void()>; 
	//函数类型：void func()，这个是【可运行函数对象】，并非【注册到线程的函数】
	Thread(ThreadCallback && cb, const string & name = "wd thread");
	//初始化必须【传入参数】：【线程对应的函数】，【线程自己的一小块char buf里要填的东西】
	~Thread();
	void start();
	void join();

private:
	static void * threadFunc(void *); //1-这款参数 void* func(void*)才是【注册到线程函数里的】

//1-看类先看私有成员
private:
	pthread_t _pthid;//2-线程id
	string _name;//3-【等待【extern __thread const char * name】往里填东西//还是它往那个写来着，等会儿看看
	bool _isRunning;//4-是否在跑状态位
	ThreadCallback _cb;//5-只是一个【运行接口】
};


}//end of namespace wd
 
 
#endif
