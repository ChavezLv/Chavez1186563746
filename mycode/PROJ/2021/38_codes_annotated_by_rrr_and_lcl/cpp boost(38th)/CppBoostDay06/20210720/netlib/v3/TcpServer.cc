 ///
 /// @file    TcpServer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 17:38:36
 ///
 

#include "TcpServer.hpp"
namespace wd
{
TcpServer::TcpServer(unsigned short port, const string ip)
: _acceptor(port, ip)
, _loop(_acceptor)
{}
	
void TcpServer::start()
{
	_acceptor.ready(); // 也可以写到构造函数里面去
	_loop.loop();
}

void TcpServer::stop()
{
	_loop.unloop();
}

void TcpServer::setAllCallbacks(TcpConnectionCallback && cb1,
			TcpConnectionCallback && cb2,
			TcpConnectionCallback && cb3)
{
	_loop.setAllCallbacks(std::move(cb1),
			std::move(cb2), 
			std::move(cb3));
}

}//end of namespace wd
