 ///
 /// @file    TestEventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 17:22:28
 ///
 
#include "TcpServer.hpp"

#include <iostream>
using std::cout;
using std::endl;
// 最后 
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
	
	//测试：回显功能
	conn->send(msg);
}

void onClose(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has closed" << endl;
}

 
void test0() 
{
	wd::TcpServer server(8888);
	server.setAllCallbacks(onConnection, onMessage, onClose);
	server.start();

} 
 
int main(void)
{
	test0();
	return 0;
}
