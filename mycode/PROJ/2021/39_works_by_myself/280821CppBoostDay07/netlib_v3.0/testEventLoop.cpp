/*================================================================
*   
*   @file       ：testEventLoop.cpp
*   @date       ：2021年08月30日 00:03:34
*   @description：
*
================================================================*/

#include "TcpServer.hpp"

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
    string msgReply("server: ");
    msgReply.append(msg);
	conn->send(msgReply);
	cout << "send: " << msgReply << endl;
}

void onClose(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has closed" << endl;
}

 
void test01() 
{
	wd::Acceptor acceptor(6666);
	acceptor.ready();


	wd::EventLoop loop(acceptor);
	loop.setAllCallbacks(onConnection, onMessage, onClose);

	loop.loop();//设置回调函数, 就可以调用这个loop方法了
} 
void test02(){
    wd::TcpServer server(6666);
    server.setAllCallbacks(onConnection, onMessage, onClose);
    server.start();
}
 
int main(void)
{
	test01();
    /* test02(); */
	return 0;
}



