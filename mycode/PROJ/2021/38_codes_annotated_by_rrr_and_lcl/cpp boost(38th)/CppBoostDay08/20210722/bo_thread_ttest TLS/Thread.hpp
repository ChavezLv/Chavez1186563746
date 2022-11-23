 ///
 /// @file    Thread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:44:17
 ///
 
#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <string>
#include <functional>
using std::string;

/*
接 上两节课 代码笔记, 在搜索引擎在线框架(***) 注释的问价夹下面
还有 39th day8 v4 也有详细笔记

(1)  头文件中写_thread char* name = "wd thread";// error, 因为thread.cc和test.cc都包含了
重复定义
而且加上一个const关键字比较好, 不能修改了
__thread const char * name = "wd thread";
(2) 能不能写成__thread const string

 */

namespace wd
{

namespace current_thread
{

extern __thread const char * name;

}

using ThreadCallback = std::function<void()>;

class Thread : Noncopyable
{
public:
	Thread(ThreadCallback && cb, const string & name="wd thread");
	~Thread();

	void start();//开启子线程
	void join();

	string getName() const {	return _name;	}

private:
	static void * threadFunc(void * arg);

private:
	pthread_t _pthid;
	string _name;
	bool _isRunning;
	ThreadCallback _cb;//代表要执行的任务
};

}//end of namespace wd
 
 
 
#endif
