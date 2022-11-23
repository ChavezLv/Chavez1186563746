/*================================================================
*   
*   @file       ：Thread.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
//对于实现文件，第一个头文件必须要是自己
//相关类型的头文件
#include "Thread.hpp"

#include <stdio.h>
#include <errno.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

namespace current_thread{

__thread const char* name = "wd thread";

}


struct ThreadData{//存在堆空间
    
    std::string _name;
    Thread::ThreadCallback _cb;

    ThreadData(const std::string &name, Thread::ThreadCallback &&cb)
    : _name(name)
    , _cb(std::move(cb)){

    }

    void runInThread(){
        /* current_thread::name = _name.c_str(); */

        wd::current_thread::name =  (_name != std::string() ? _name.c_str() : "wd thread");

        if(_cb){
            _cb();//调用回调函数
        }
    }


};

//Thread(const ThreadCallback &);

Thread::Thread(ThreadCallback && cb, const string &name)
: _pthid(0)
, _name(name)// 表示线程的名字
, _isRunning(false)
, _cb(std::move(cb))
{}

Thread::~Thread()
{
	if(_isRunning) {
		pthread_detach(_pthid);
		_isRunning = false;
	}
	cout << "~Thread()" << endl;
}

void Thread::start()
{
    ThreadData *pdata = new ThreadData(_name, std::move(_cb));
    

	//需要一个线程入口函数
	if(pthread_create(&_pthid, nullptr,  threadFunc, pdata) != 0) {
		perror("pthread_create");	
		return;
	}
	_isRunning = true;
}

void * Thread::threadFunc(void * arg)
{//子线程的入口函数
 //在这里要调用run方法
        
	 ThreadData * pdata = static_cast<ThreadData*>(arg);
	 if(pdata) {
		pdata->runInThread();
	 }

     delete pdata;//堆空间, 释放掉
	 return nullptr;
}

void Thread::join()
{
	if(_isRunning) {
		//pthread_join是一个阻塞式函数
		pthread_join(_pthid, nullptr);

		_isRunning = false;
	}	
}

}//end of namespace wd

