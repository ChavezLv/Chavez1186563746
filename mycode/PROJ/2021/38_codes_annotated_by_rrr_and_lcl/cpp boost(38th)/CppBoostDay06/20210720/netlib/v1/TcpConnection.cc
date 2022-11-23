 ///
 /// @file    TcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-16 16:10:24
 ///
 
#include "TcpConnection.hpp"

#include <string.h>
#include <sstream>

namespace wd
{

TcpConnection::TcpConnection(int fd)
: _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
, _shutdownWrite(false)
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
