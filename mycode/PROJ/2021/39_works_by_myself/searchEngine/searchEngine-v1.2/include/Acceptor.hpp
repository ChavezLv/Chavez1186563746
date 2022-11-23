/*================================================================
*   
*   @file       ：Acceptor.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_ACCEPTOR_HPP__
#define __WD_ACCEPTOR_HPP__

#include "InetAddress.hpp"
#include "Socket.hpp"

namespace wd
{

class Acceptor
{
public:
	/* Acceptor(unsigned short port, const string & ip = "127.0.0.1") */
	Acceptor(unsigned short port, const string & ip = "192.168.10.132")
	: _listensock()
	, _addr(port, ip)
	{}

	void ready();
	int accept();

	int fd() const {	return _listensock.fd();	}
private:
	void setReuseAddr();
	void setReusePort();
	void bind();
	void listen();

private:
	Socket _listensock;
	InetAddress _addr;
};

}//end of namespace wd
 
 
 
#endif
