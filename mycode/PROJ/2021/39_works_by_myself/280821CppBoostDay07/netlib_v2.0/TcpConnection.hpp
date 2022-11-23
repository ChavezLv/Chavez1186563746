/*================================================================
*   
*   @file       ：TcpConnection.hpp
*   @date       ：2021年08月29日 21:52:10
*   @description：
*
================================================================*/


#ifndef __TCPCONNECTION_HPP__
#define __TCPCONNECTION_HPP__

#include "Noncopyable.hpp"
#include "Socket.hpp"
#include "InetAddress.hpp"
#include "SocketIO.hpp"

#include <memory>
#include <functional>
using std::shared_ptr;
using std::function;

namespace wd{

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

class TcpConnection
: public Noncopyable
, public std::enable_shared_from_this<TcpConnection>{
public:

    TcpConnection(int fd)
    : _socket(fd)
    , _socketIO(fd)
    , _localAddr(getlocalAddr())
    , _peerAddr(getPeerAddr())
    , _shutdownWrite(false){

    }
    
    ~TcpConnection() {  }

    string toString() const;

    string receive();
    void send(const string & msg);

    void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();


    void setAllCallbacks(const TcpConnectionCallback & cb1,
			const TcpConnectionCallback & cb2,
			const TcpConnectionCallback & cb3)
	{
		_onConnection = cb1;
		_onMessage = cb2;
		_onClose = cb3;
	}

	bool isClosed() const;

private:
    InetAddress getlocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _socket;
    SocketIO _socketIO; 
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _shutdownWrite;

    TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;
};

}//end of namespace wd

#endif //TCPCONNECTION_HPP
