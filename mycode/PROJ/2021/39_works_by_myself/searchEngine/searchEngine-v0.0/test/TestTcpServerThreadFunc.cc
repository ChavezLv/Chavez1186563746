/*================================================================
*   
*   @file       ：TestTcpServer.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "Threadpool.hpp"
#include "TcpServer.hpp"
/* #include "Thread.hpp" // 因为Threadpool.hpp没有*/
#include <unistd.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using namespace wd;

class Task
{
public:
	Task(const string & msg, const TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{
	
	}
    void print(){
        cout << "print()>>sub thread name : " << wd::current_thread::name << endl;
		cout << "Task::print() is running" << endl;
		string response = _msg;//模拟回显服务
		_conn->sendInLoop(response);
    }

	//process方法的执行在线程池中的某一个子线程进行
	void process()
	{
        cout << "process()>>sub thread name : " << wd::current_thread::name << endl;
		cout << "Task::process() is running" << endl;
		//1. 执行业务逻辑的处理
		//decode
		//compute
		//encode

		//2. 将结果发送给客户端
		string response = _msg;//模拟回显服务
		//如果该操作能够正常执行，那就说明是在线程池中完成了
		//数据的发送，与模型不符合
		//
		//模型中，发送的数据的操作是在Reactor中完成的，
		//是在事件循环中完成的
		//
		//我们应该在这里，通知Reactor(IO线程)完成数据的发送
		//该怎么做？此时在计算线程中拥有的资源只有TcpConneciton对象
		//因此要通知IO线程，只能借助与TcpConnection对象
		//
		//当通知完成之后，Reactor要执行的任务是什么呢？
		//就是发送数据，即send方法的执行
		//那就需要将send方法包装成一个回调函数，注册给
		//Reactor对象所在的线程
		/* _conn->send(response); */
		_conn->sendInLoop(response);
	}

private:
	string _msg;
	TcpConnectionPtr _conn;
};

wd::Threadpool * gThreadpool = nullptr;

//网络编程过程中涉及到的三个事件
void onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
}

void func(int nums, const string &msg, TcpConnectionPtr &conn){
    cout << "func:: " << nums << endl; 
    
    string response = msg;//模拟回显服务
    conn->sendInLoop(response);
}

void onMessage(const TcpConnectionPtr & conn)
{
	string msg = conn->receive();
	cout << "receive msg :" << msg << endl;
	//执行业务逻辑的处理的
	//但这个部分执行的时间不能过长， 10ms
	//1. 消息的处理最好是交给线程池来执行
        Task task(msg, conn);
    if(msg == string("1\n")){
        cout << "msg is 1" << endl;
        gThreadpool->addTask(std::bind(&Task::print, task));	

    }else if(msg == string ("2\n")){
        cout << "msg is 2" << endl;
        gThreadpool->addTask(std::bind(&Task::process, task));	

    }else{
        cout << "msg is 3" << endl;
        gThreadpool->addTask(std::bind(func, 100, msg,conn));	

    }
	//2. 得到要返回给客户端的结果后，
	//再发送该返回结果
	//conn->send(msg);
}
 
void onClose(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has closed!" << endl;
}

void test0() 
{
	using namespace wd;
	::signal(SIGPIPE, SIG_IGN);

	Threadpool threadpool(4, 10);
	threadpool.start();
	gThreadpool = &threadpool;

	TcpServer server(8888);
	server.setAllCallbacks(onConnection, onMessage, onClose);
	server.start();
} 


class EchoServer
{
public:
	EchoServer(unsigned short port, const string & ip = "127.0.0.1")
	: _threadpool(4, 10) // 4, 10最好写在配置文件中
	, _server(port)
	{}

	void start() {
		_threadpool.start();
		_server.setAllCallbacks(
			std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
			std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
			std::bind(&EchoServer::onClose, this, std::placeholders::_1));

		_server.start();
	}

	void onConnection(const TcpConnectionPtr & conn)
	{
		cout << ">> tcp " << conn->toString() << " has connected!" << endl;
	}

	void onMessage(const TcpConnectionPtr & conn)
	{
		string msg = conn->receive();
		//加入协议解析的代码
		Task task(msg, conn);
		_threadpool.addTask(std::bind(&Task::process, task));
	}

	void onClose(const TcpConnectionPtr & conn)
	{
		cout << ">> tcp " << conn->toString() << " has closed!" << endl;
	}

private:
	Threadpool _threadpool;
	TcpServer _server;
};

void test1()
{
	EchoServer server(8888);
	server.start();
}
 
int main(void)
{
	test0();
	/* test1(); */
	return 0;
}
