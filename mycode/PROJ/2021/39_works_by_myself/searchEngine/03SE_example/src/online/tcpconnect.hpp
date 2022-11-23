#ifndef __RXL_TCPCONNECT_H__
#define __RXL_TCPCONNECT_H__
#include"noncopy.hpp"
#include"socket.hpp"
#include"socketio.hpp"
#include"inetaddress.hpp"
#include<functional>
#include<memory>
namespace rxl{

class EventPoll;

class TcpConnect
:Noncopy
,public std::enable_shared_from_this<TcpConnect>
{
public:
	using TcpconnectionPtr = std::shared_ptr<TcpConnect>;
	using EpollCallBack = std::function<void(const TcpconnectionPtr&)>;
	TcpConnect(int fd,EventPoll& eventpoll);
	~TcpConnect();
	string receive();
	void send(const string & msg);
	string toString() const;
	void shutdown();
	void setConnectCallBack(const EpollCallBack&);
	void setMessageCallBack(const EpollCallBack&);
	void setCloseCallBack(const EpollCallBack&);
	void handleConnectionCallBack();
	void handleMessageCallBack();
	void handleCloseCallBack();
	void sendInloop(const string&);
private:
	InetAddress getLocalAddr(int fd);
	InetAddress getPeerAddr(int fd);
	EpollCallBack _onConnection;
	EpollCallBack _onMessage;
	EpollCallBack _onClose;
private:
	Socket _sock;
	SocketIO _socketIo;
	InetAddress _localAddr;
	InetAddress _peerAddr;
	bool _isShutdwonWrite;
	EventPoll& _eventpoll;
};
}//end of namespace
#endif
