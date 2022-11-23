/*================================================================
*   
*   @file       ：TcpConnection.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_TCPCONNECTION_HPP__
#define __WD_TCPCONNECTION_HPP__

#include "Noncopyable.hpp"

#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"

#include <functional>
#include <memory>

using std::function;
using std::shared_ptr;

namespace wd
{

class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;
using TcpConnectionCallbackParse = function<void(const TcpConnectionPtr&, const std::pair<int, std::string>& )>;

class TcpConnection 
: Noncopyable
, public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(int fd, EventLoop * loop);
	~TcpConnection();

	string receive();
	void send(const string & msg);
	void sendInLoop(const string & msg);
    void sendInLoopParse(const std::pair<int, std::pair<int, std::string> > &msg);

	string toString() const;

	void setAllCallbacks(const TcpConnectionCallback & cb1,
			const TcpConnectionCallback & cb2,
			const TcpConnectionCallback & cb3,
			const TcpConnectionCallbackParse & cb4)
	{
		_onConnection = cb1;
		_onMessage = cb2;
		_onClose = cb3;
		_onParsers = cb4;
	}

	void handleConnectionCallback();
	void handleMessageCallback();
	void handleParsersCallback(const std::pair<int, std::string> &msgtype);
	void handleCloseCallback();

	bool isClosed() const;

    int fd() const {return _sock.fd(); }

private:
	InetAddress getLocalAddr();
	InetAddress getPeerAddr();

private:
	Socket _sock;
	SocketIO _sockIO;
	InetAddress _localAddr;
	InetAddress _peerAddr;
	bool _shutdownWrite;
	EventLoop * _loop;

	TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;
	TcpConnectionCallbackParse _onParsers;
};

}//end of namespace wd
 
 
 
#endif
