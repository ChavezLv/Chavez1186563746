 ///
 /// @file    TcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-16 16:10:24
 ///
 
#include "TcpConnection.hpp"
#include "EventLoop.hpp"

#include <string.h>
#include <sstream>

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

void TcpConnection::setAllCallbacks(
		const TcpConnectionCallback & cb1,
		const TcpConnectionCallback & cb2,
		const TcpConnectionCallback & cb3)
{
	_onConnectionCb = std::move(cb1);
	_onMessageCb = std::move(cb2);
	_onCloseCb = std::move(cb3);
}

bool TcpConnection::isClosed() const
{
	int nret = -1;
	char buff[64] = {0};
	do{
		nret = ::recv(_sock.fd(), buff, sizeof(buff), MSG_PEEK);
	}while(nret == -1 && errno == EINTR);

	return nret == 0;
}

void TcpConnection::handleConnectionCallback()
{
	if(_onConnectionCb) {
		_onConnectionCb(shared_from_this());
	}
}

void TcpConnection::handleMessageCallback()
{
	if(_onMessageCb) {
		_onMessageCb(shared_from_this());
	}
}

void TcpConnection::handleCloseCallback()
{
	if(_onCloseCb) {
		_onCloseCb(shared_from_this());
	}
}

InetAddress TcpConnection::getPeerAddr()
{
	struct sockaddr_in localaddr;
	socklen_t len = sizeof(localaddr);
	::memset(&localaddr, 0, sizeof(localaddr));
	if(getpeername(_sock.fd(), (struct sockaddr*)&localaddr, &len) < 0) {
		perror("getsockname");
	}
	return InetAddress(localaddr);
}

string TcpConnection::toString() const
{
	std::ostringstream oss;
	oss << "tcp " 
		<< _localAddr.ip() << ":" << _localAddr.port() << " --> "
		<< _peerAddr.ip() << ":" << _peerAddr.port();
	return oss.str();
}

}//end of namesapce wd
