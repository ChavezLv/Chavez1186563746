 ///
 /// @file    TestEventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-17 16:10:01
 ///
 
#include "TcpServer.hpp"
#include "Threadpool.hpp"

#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using namespace wd;

void cb1(const TcpConnectionPtr & conn) 
{
	//连接建立时要记录日志
	cout << conn->toString() << " has connected!" << endl;
} 
/*lcl: 后面涉及到缓存系统的设计,  笔记记载V4那里*/
class MyTask
{
public:
	MyTask(const string & msg,
		const TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{}

	//process方法的执行一定是在某一个计算线程中运行
	void process()
	{
		cout << "Task::process msg:" << _msg << endl;
		//...
		//decode
		//compute
		//encode
		//回显服务  //发送消息
		//_conn->send(_msg);//按照职责的划分，计算线程并不负责数据的发送操作
						  //在这里计算线程应该要通知IO线程进行数据的发送
						  //使用eventfd来完成通知操作
		//唤醒IO线程, 就是EventLoop对象在loop方法中运行时调用send方法-->
		//1. 让epoll_wait返回  ==> 监听evenfd文件描述符
		//2. 之后再去执行_cond->send方法
		//
		//如何获取EventLoop对象?
		//只有TcpConnection知道被哪一个EventLoop进行管理
		/* wakeup(); */

		////1. 拿到关键词之后，要先从缓存系统中查找
		//如何拿到缓存呢？ LRUCache ==> CacheManger中用vector存储
		// ==> 只需要获取到一个vector的下标就可以定位一个LRUCache
		
		//可行的方案
		//每一个线程的id是固定的   pthead_t --> LRUCache
		//map<pthread_t, int> _id2idxs;
		//pthread_self();
	
		
		//LRUCache & cache = CacheManager::getInstance()->getCache(pthread_self());
		//map<pthread_t, int>
		//LRUCache & cache = CacheManager::getInstance()->getCache(_id2idxs[pthread_self()]);
		
		int idx = atoi(wd::current_thread::name);
		cout << "idx : " << idx << endl;
		//LRUCache & cache = CacheManager::getInstance()->getCache(threadIdx);
		//---->[R/★]也就是说【几个cache】，实际上是【被cache类拥有的】，线程只有【依据自己的id拿到它们的资格】
		//存储线程对象为什么做不到这件事？因为【放一个index全局变量in线程】非常不优雅不美观啊！
		//
		//要想完美的解决该问题，就需要使用到一个新知识TLS（线程局部存储）--->线程的全局变量

		
		//2. 缓存中没有的时候，再去进行查询流程的执行

		_conn->sendInLoop(_msg);
	}

private:
	string _msg;
	TcpConnectionPtr _conn;
};

Threadpool * gThreadpool = nullptr;

void cb2(const TcpConnectionPtr & conn)
{
	//消息到达时，要进行业务逻辑的处理
	string msg = conn->receive();//获取了消息

	//decode
	//compute
	//encode
	//这些操作要看成是一个任务
	MyTask task(msg, conn);
	gThreadpool->addTask(std::bind(&MyTask::process, task));

}

void cb3(const TcpConnectionPtr & conn)
{
	//连接断开时要记录日志
	cout << conn->toString() << " has closed!" << endl;
}
 
int main(void)
{
	/* cb1(); */
	/* cb2(); */
	/* cb3(); */

	Threadpool threadpool(4, 10);
	threadpool.start();
	gThreadpool = &threadpool;

	TcpServer server(8888);
	server.setAllCallbacks(cb1, cb2, cb3);
	server.start();

	return 0;
}
