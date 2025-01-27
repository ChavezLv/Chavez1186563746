 ///
 /// @file    TcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 17:12:51
 ///
 

#include "TcpConnection.h"
#include "InetAddress.h"
#include "EventLoop.hpp"

#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <sstream>

namespace wd
{
TcpConnection::TcpConnection(int fd,EventLoop*pEventLoop)
:_eventloop(pEventLoop)
, _sock(fd)
, _socketIo(fd)
, _localAddr(getLocalAddr(fd))
, _peerAddr(getPeerAddr(fd))
, _isShutdwonWrite(false)

{

}

TcpConnection::~TcpConnection()
{
	if(!_isShutdwonWrite) {
		shutdown();
	}
}

string TcpConnection::receive()
{
	char buff[65536] = {0};
	int recvChar=_socketIo.readline(buff, sizeof(buff));
	
    cout<<"recvword is "<<buff<<endl;
    cout<<"recvChar is "<<recvChar<<endl;
    return string(buff);
}
	
void TcpConnection::send(const string & msg)
{
	_socketIo.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInloop(const string& msg)
{
    _eventloop->runInLoop(bind(&TcpConnection::send,this,msg));
}

void TcpConnection::shutdown()
{
	if(!_isShutdwonWrite) {
		_isShutdwonWrite = true;
		_sock.shutdownWrite();
	}
}

string TcpConnection::toString() const
{
	std::ostringstream oss;
	oss << _localAddr.ip() << ":" << _localAddr.port() << " --> "
		<< _peerAddr.ip() << ":" << _peerAddr.port();
	return oss.str();
}


InetAddress TcpConnection::getLocalAddr(int fd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr);
	if(getsockname(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
		perror("getsockname");
	}
	return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(int fd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr);
	if(getpeername(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
		perror("getpeername");
	}
	return InetAddress(addr);
}

}//end of namespace wd
