 ///
 /// @file    TcpServer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 17:35:41
 ///
 
#ifndef __WD_TCPSERVER_HPP__
#define __WD_TCPSERVER_HPP__

#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "TcpConnection.hpp"


namespace wd
{

class TcpServer
{
public:
	TcpServer(unsigned short port, const string ip = "127.0.0.1");
	void start();
	void stop();
	void setAllCallbacks(TcpConnectionCallback && cb1,
			TcpConnectionCallback && cb2,
			TcpConnectionCallback && cb3);

private:
	Acceptor _acceptor;
	EventLoop _loop;
};




}//end of namespace wd
 
 
 
#endif
