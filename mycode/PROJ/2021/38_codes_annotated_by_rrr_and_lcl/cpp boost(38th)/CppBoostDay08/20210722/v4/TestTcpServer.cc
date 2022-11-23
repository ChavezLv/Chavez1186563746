 ///
 /// @file    TestEventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 17:22:28
 ///
 
#include "TcpServer.hpp"
#include "Threadpool.hpp"

#include <iostream>
using std::cout;
using std::endl;


class Mytask
{
public:
	Mytask(const string & msg, const wd::TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{}
	
	//计算线程  一个线程对应一个Cache  一一对应的关系
	void process()
	{
		//....在线程池中的某一个子线程(计算线程)进行
		cout << "Mytask::process()" << endl;
		//decode
		//compute == > 重复进行计算   vector<Cache <key, value>>  ==> pthread_self() => map<pthread_t, int>
		//endcode   //线程局部存储
		//得到了要发送给对端的结果
		string response = _msg;//假设得到了一个消息
		
		//(1) 以下不符合模型设计
		//_conn->send(response);//数据的发送是直接在计算线程中进行的
		//计算线程的职责是处理业务逻辑，而不是数据的发送
		//此时，应该将数据发送的任务交给IO线程去做.,必须需要
		//去通知IO线程进行发送
		
		//(2)
		//wakeup 通知IO线程, 并且把send方法的执行延迟到IO线程中进行
		//必须要将send方法作为一个回调函数注册给IO线程
		_conn->sendInLoop(response);
	}

private:
	string _msg;//放消息本身
	wd::TcpConnectionPtr _conn;
};

wd::Threadpool * gThreadpool = nullptr;

void onConnection(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
}

void onMessage(const wd::TcpConnectionPtr & conn)
{
	string msg = conn->receive();
	//decode
	//compute
	//encode
	//以上这三个步骤执行的时间不宜过长(10ms)
	//
	//当执行时间过长时，最好的方式就是将其封装成一个任务
	//交给线程池去进行处理
	
	Mytask task(msg, conn);
	gThreadpool->addTask(std::bind(&Mytask::process, task));
	//测试：回显功能
	//conn->send(msg);
}

void onClose(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has closed" << endl;
}

 
void test0() 
{
	wd::Threadpool threadpool(4, 100);
	threadpool.start();

	gThreadpool = &threadpool;

	wd::TcpServer server(8888);
	server.setAllCallbacks(onConnection, onMessage, onClose);
	server.start();
} 
 
int main(void)
{
	test0();
	return 0;
}
