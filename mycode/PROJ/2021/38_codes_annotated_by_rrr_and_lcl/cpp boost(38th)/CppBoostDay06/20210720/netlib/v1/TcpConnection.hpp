 ///
 /// @file    TcpConnection.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-16 16:03:56
 ///
 
#ifndef __WD_TCPCONNECTION_HPP__
#define __WD_TCPCONNECTION_HPP__

#include "Noncopyable.hpp"

#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"


namespace wd
{

class TcpConnection : Noncopyable
{
public:
	TcpConnection(int fd);
	~TcpConnection();

	string receive();
	void send(const string & msg);

	string toString() const;

private:
	InetAddress getLocalAddr();
	InetAddress getPeerAddr();

private:
	Socket _sock;
	SocketIO _sockIO;
	InetAddress _localAddr;
	InetAddress _peerAddr;
	bool _shutdownWrite;
};

}//end of namespace wd
 
 
 
#endif
