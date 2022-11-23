#ifndef __RXL_TCPSERVER_H__
#define __RXL_TCPSERVER_H__
#include"tcpconnect.hpp"
#include"accept.hpp"
#include"eventpoll.hpp"
namespace rxl{

class TcpServer{
	using TcpConnectionPtr = std::shared_ptr<TcpConnect>;
	using CallBack = function<void(const TcpconnectionPtr&)>;
public:
	TcpServer(string ip,unsigned short port)
	:_acceptor(ip.c_str(),port)
	,_eventpoll(_acceptor)
	{}
	void start(){
		_acceptor.ready();
		_eventpoll.loop();
	}
	void setConnectCallBack(CallBack&& cb){
		_eventpoll.setConnectCallBack(std::move(cb));
	}
	void setMessageCallBack(CallBack&& cb){
		_eventpoll.setMessageCallBack(std::move(cb));
	}
	void setCloseCallBack(CallBack&& cb){
		_eventpoll.setCloseCallBack(std::move(cb));
	}

private:
	Acceptor _acceptor;
	EventPoll _eventpoll;
};
}//end of namespace 
#endif 
