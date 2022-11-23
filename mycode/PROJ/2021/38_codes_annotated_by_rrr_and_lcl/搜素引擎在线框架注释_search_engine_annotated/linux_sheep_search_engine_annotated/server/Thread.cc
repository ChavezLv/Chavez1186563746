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
 /*
接 上两节课 代码笔记, 在搜索引擎在线框架(***) 注释的问价夹下面
还有 39th day8 v4 也有详细笔记

(1)  头文件中写_thread char* name = "wd thread";// error, 因为thread.cc和test.cc都包含了
重复定义
而且加上一个const关键字比较好
__thread const char * name = "wd thread";

 */
namespace wd
{

namespace current_thread 
{
__thread const char * name = "wd thread";	
//默认这块【线程局部存储（TLS: Thread-Local Storage）】最开始存的值是"wd thread"
}
//using ThreadCallback = std::function<void()>//在.hpp文件里
using ThreadCallback = Thread::ThreadCallback ;//这个就是【实际填写到thread_call_back】里的函数 

struct ThreadData//本结构【存储在堆空间】，【不存储在实际的类的栈空间里】

{
	string _name;//一个名字的string
	ThreadCallback _cb;//一个回调函数入口

	ThreadData(const string & name, ThreadCallback && cb)//一个初始化构造函数
	: _name(name)
	, _cb(std::move(cb))
	{}

	void runInThread()//开始跑//【线程的运行在这个类里】
	{
		current_thread::name = _name.c_str();//1-把姓名填入TLS
		//从指针到指针
		if(_cb) _cb();//函数对象，是【一种入口】，【被回调函数注册函数注册的东西，因为<它是一个指针>】
		//2-运行【真正要运行的函数】--->BO的精髓
	}
};


Thread::Thread(ThreadCallback && cb, const string & name)
: _pthid(0)//pthread_id运行之前也没有
, _name(name)//需要【为name属性赋值】
, _isRunning(false)//构造出来的时候还没start
, _cb(std::move(cb))//注册【thread运行的函数】//[1]cb初次出现的地方//cb是要运行的东西
{}

Thread::~Thread()
{
	if(_isRunning) {//主线程的析构就是【主线程和子线程解除关联】//这个不是join
		pthread_detach(_pthid);
		_isRunning = false;
	}
}

void Thread::start()//这个不是静态函数，是基于【某个线程对象】的
{
	ThreadData * pdata = new ThreadData(_name, std::move(_cb));//该线程对象【在中间new出来了pdata】做线程传参用
	//[2]cb被注册到了【pthread_create启动起来的function】【拿得到的指针】里
	//--->pthread现在启动起来的函数里，是【支持cpp对象】的//--->所以无形之中，参数的功能被大大拓展了
	int nret = 0;
	if((nret = pthread_create(&_pthid, nullptr, threadFunc, pdata)) != 0) {//在start函数会启动线程
		//函数是静态函数，但是【用的参数是】
		perror("pthread_create");
		fprintf(stderr, "thread error:%s\n", strerror(nret));
		return;
		//线程在start起来了，且【拿到了一个ThreadData对象】，这个对象
		//1-享有一个独有的名字(之后赋值给TLS) 2-里面有一个cb()运行的仿函数对象（相当于在线程启动func里再调用cb()）
	}
	_isRunning = true;//且修改状态位
}

//子线程的入口
void * Thread::threadFunc(void * arg)//这才是注册到pthread_create里的
{
	//要执行任务，就必要调用run方法, 此时必须要知道Thread对象
	ThreadData * pdata = static_cast<ThreadData*>(arg);//这个类型本质指向【功能健全的对象】
	if(pdata) {
		pdata->runInThread();//1-string写入【线程TLS】 2-运行cb()//有意思，确实更方便了
	}
	delete pdata;

	return nullptr;
}

void Thread::join()//join工具
{
	if(_isRunning) {
		pthread_join(_pthid, nullptr);
		_isRunning = false;
	}
}

}//end of namespace wd
