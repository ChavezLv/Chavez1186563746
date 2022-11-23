#ifndef __RXL_ACCEPT_H__
#define __RXL_ACCEPT_H__
#include"socket.hpp"
#include"inetaddress.hpp"
#include<string>
using std::string;
namespace rxl{
class Acceptor{
public:
	Acceptor(unsigned short port)
	:_addr(port)
	,_listenfd()
	{}
	Acceptor(const string ip,unsigned short port)
	:_addr(ip,port)
	,_listenfd()
	{}
	void ready(){
		setReuseAddr(true);
		setReusePort(true);
		bind();
		listen();
	}
	int accept(){
		int newfd = ::accept(_listenfd.fd(),NULL,NULL);
		if(newfd == -1){
			perror("accept");
		}
		return newfd;
	}
	int fd() const{ return _listenfd.fd(); }
private:
	void bind(){
		int ret = ::bind(_listenfd.fd(),_addr.getSockAddressPtr(),sizeof(struct sockaddr));
		if(ret == -1){
			perror("tcp bind");
		}
	}
	void listen(){
		int ret = ::listen(_listenfd.fd(),10);
		if(ret == -1){
			perror("listen");
		}
	}
	void setReusePort(bool flag){
		int one = flag;
		int ret = setsockopt(_listenfd.fd(),SOL_SOCKET, SO_REUSEADDR,&one,sizeof(one));
		if(ret < 0){
			perror("setsockopt setReuseAddr");
		}	
	}
	void setReuseAddr(bool flag){
		int one = flag;
		int ret = setsockopt(_listenfd.fd(),SOL_SOCKET, SO_REUSEPORT,&one,sizeof(one));
		if(ret < 0){
			perror("setsockopt setReusePort");
		}
	}
private:
	InetAddress _addr;
	Socket _listenfd;
};
}//end of namespace
#endif
