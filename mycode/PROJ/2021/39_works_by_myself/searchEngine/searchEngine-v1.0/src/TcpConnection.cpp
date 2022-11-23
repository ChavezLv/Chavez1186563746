/*================================================================
*   
*   @file       ：TcpConnection.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "TcpConnection.hpp"
#include "EventLoop.hpp"

#include <string.h>
#include <sstream>
#include <iostream>

namespace wd
{

TcpConnection::TcpConnection(int fd, EventLoop * loop)
: _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
, _shutdownWrite(false)
, _loop(loop)
{}

TcpConnection::~TcpConnection()
{
//...
}

string TcpConnection::receive()
{
	char buff[65536] = {0};
	_sockIO.readline(buff, sizeof(buff));
	return string(buff);
}

void TcpConnection::send(const string & msg)
{
	_sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string & msg)
{
	if(_loop) {
		_loop->runInLoop(std::bind(&TcpConnection::send, this, msg));	
	}
}
void TcpConnection::sendInLoopParse(const std::pair<int, std::pair<int, std::string> > &msg){
    if(_loop) {
/* std::cout << "sendInLoopParse()" << std::endl; */
        _loop->EventLoop::parseInLoop(msg);
    }
}

InetAddress TcpConnection::getLocalAddr()
{
	struct sockaddr_in localaddr;
	socklen_t len = sizeof(localaddr);
	::memset(&localaddr, 0, sizeof(localaddr));
	if(getsockname(_sock.fd(), (struct sockaddr*)&localaddr, &len) < 0) {
		perror("getsockname");
	}
	return InetAddress(localaddr);
}

InetAddress TcpConnection::getPeerAddr()
{
	struct sockaddr_in peeraddr;
	socklen_t len = sizeof(peeraddr);
	::memset(&peeraddr, 0, sizeof(peeraddr));
	if(getpeername(_sock.fd(), (struct sockaddr*)&peeraddr, &len) < 0) {
		perror("getsockname");
	}
	return InetAddress(peeraddr);
}

string TcpConnection::toString() const
{
	std::ostringstream oss;
	oss << "tcp " 
		<< _localAddr.ip() << ":" << _localAddr.port() << " --> "
		<< _peerAddr.ip() << ":" << _peerAddr.port();
	return oss.str();
}

void TcpConnection::handleConnectionCallback()
{
	if(_onConnection) {
		_onConnection(shared_from_this());
	}
}

void TcpConnection::handleMessageCallback()
{
	if(_onMessage) {
		_onMessage(shared_from_this());
	}
}
void TcpConnection::handleParsersCallback(const std::pair<int, std::string>& msgtype){
    if(_onParsers){
        /* _onParsers(shared_from_this(), msgtype); */
        _onParsers(shared_from_this(), msgtype);
    }
}

void TcpConnection::handleCloseCallback()
{
	if(_onClose) {
		_onClose(shared_from_this());
	}
}

bool TcpConnection::isClosed() const 
{
	int nret = -1;
	char buff[64] = {0};
	do {
		//通过查看内核缓冲区中的数据，不移走数据
		nret = ::recv(_sock.fd(),  buff, sizeof(buff), MSG_PEEK|MSG_DONTWAIT);
	}while(nret == -1 && errno == EINTR);

	return nret == 0;
}

}//end of namesapce wd
