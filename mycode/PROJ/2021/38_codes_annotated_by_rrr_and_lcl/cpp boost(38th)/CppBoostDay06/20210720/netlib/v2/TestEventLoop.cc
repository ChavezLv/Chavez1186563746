 ///
 /// @file    TestEventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 17:22:28
 ///
 
#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "TcpConnection.hpp"

#include <iostream>
using std::cout;
using std::endl;

//以后就不用去修改框架了, 想要干什么就直接在这里修改, 但是现在还是要
// 引入3个头文件, 还要命名多个对象, 所以再改进一下, 用组合
// 组合到一个类里面TcpServer
// TcpServer类最后一次进行封装
void onConnection(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
}

void onMessage(const wd::TcpConnectionPtr & conn)
{
	string msg = conn->receive();
	cout << "received: " << msg << endl;
	//decode
	//compute
	//encode
	
	//测试：回显功能( 执行回显操作, 先去看一下网络有没有通)
	conn->send(msg);
}

void onClose(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has closed" << endl;
}

 
void test0() 
{
	wd::Acceptor acceptor(8888);
	acceptor.ready();

	wd::EventLoop loop(acceptor);
	loop.setAllCallbacks(onConnection, onMessage, onClose);

	loop.loop();//设置号回调函数, 就可以调用这个loop方法了
} 
 
int main(void)
{
	test0();
	return 0;
}
