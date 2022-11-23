#include"tcpconnect.hpp"
#include "eventpoll.hpp"
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
using std::endl;
using std::cout;
namespace rxl{
	TcpConnect::TcpConnect(int fd,EventPoll& eventpoll)
	:_sock(fd)
	,_socketIo(fd)
	,_localAddr(getLocalAddr(fd))
	,_peerAddr(getPeerAddr(fd))
	,_isShutdwonWrite(false)
	,_eventpoll(eventpoll)
	{}
	TcpConnect::~TcpConnect(){
		if(!_isShutdwonWrite){
			shutdown();
		}
	}
	string TcpConnect::receive(){
		char buf[65536] = {0};
		_socketIo.readline(buf,sizeof(buf));
		return string(buf);
	}
	void TcpConnect::send(const string & msg){
		_socketIo.writen_train(msg.c_str(),msg.size());
	}

	void TcpConnect::sendInloop(const string& msg){
		_eventpoll.pushInSolveList(std::bind(&TcpConnect::send,this,msg));
	}

	string TcpConnect::toString() const{
		std::ostringstream oss;
		oss<< _localAddr.ip() << ":" << _localAddr.port() << " --> "
			<< _peerAddr.ip() << ":" << _peerAddr.port();
		return oss.str();
	}
	void TcpConnect::shutdown(){
		if(!_isShutdwonWrite){
			_sock.shutdownWrite();
		}
	}
	InetAddress TcpConnect::getLocalAddr(int fd){
		struct sockaddr_in addr;
		socklen_t len = sizeof(struct sockaddr);
		if(getsockname(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
			perror("getsockname");
		}
		return InetAddress(addr);
	}
	InetAddress TcpConnect::getPeerAddr(int fd){
		struct sockaddr_in addr;
		socklen_t len = sizeof(struct sockaddr);
		if(getpeername(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
			perror("getsockname");
		}
		return InetAddress(addr);
	}
	void TcpConnect::setConnectCallBack(const EpollCallBack& cb){
		_onConnection = std::move(cb);
	}
	void TcpConnect::setMessageCallBack(const EpollCallBack& cb){		
		_onMessage = std::move(cb);
	}
	void TcpConnect::setCloseCallBack(const EpollCallBack& cb){
		_onClose = std::move(cb);
	}
	void TcpConnect::handleConnectionCallBack(){
		if(_onConnection){
			_onConnection(shared_from_this());
		}
	}
	void TcpConnect::handleMessageCallBack(){
		if(_onMessage){
			_onMessage(shared_from_this());
		}
	}
	void TcpConnect::handleCloseCallBack(){
		if(_onClose){
			_onClose(shared_from_this());
		}
	}
}//end of namespace
