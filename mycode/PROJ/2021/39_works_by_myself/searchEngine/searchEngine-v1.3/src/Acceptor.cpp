/*================================================================
*   
*   @file       ：Acceptor.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "Acceptor.hpp"

namespace wd
{
void Acceptor::ready()
{
	setReuseAddr();
	setReusePort();
	bind();
	listen();
}

void Acceptor::setReuseAddr()
{
	int on = 1;
	if(setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt");		
	}
}

void Acceptor::setReusePort()
{
	int on = 1;
	if(setsockopt(fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0) {
		perror("setsockopt");		
	}
}

void Acceptor::bind()
{
	if(::bind(fd(), (struct sockaddr*)_addr.getInetAddressPtr(), sizeof(_addr)) < 0 ) {
		perror("bind");
	}
}

void Acceptor::listen()
{
	if(::listen(fd(), 128) < 0) {
		perror("listen");
	}
}

int Acceptor::accept()
{
	int peerfd = ::accept(fd(), nullptr, nullptr);
	if(peerfd < 0) {
		perror("accept");
	}
	return peerfd;
}



}//end of namespace wd
