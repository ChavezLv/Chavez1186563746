#ifndef __RXL_THREAD_HPP__
#define __RXL_THREAD_HPP__
#include"noncopy.hpp"
#include<pthread.h>
#include<string>
#include<functional>
#include<iostream>
using std::string;
namespace rxl{
	using ThreadCallBack = std::function<void()>;
namespace currentThread{
   	extern __thread const char* threadName ;
}//end of namespace currentThread

class Thread
:Noncopy
{
public:
	Thread(ThreadCallBack&& cb,const string& name = string())
	:_isRunning(false)
 	,_tid(0) 
	,_cb(std::move(cb))
	,_name(name)
	{}
	~Thread();
	void create();
	void join();
private:
	static void* Run(void* args);
private:
	bool _isRunning;
	pthread_t _tid;
	ThreadCallBack _cb;
	string _name;
};
}//end of namespace
#endif
