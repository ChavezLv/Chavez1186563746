#ifndef __tcpConn_H__
#define __tcpConn_H__

#include"Socket.h"
#include"SocketIO.h"
#include"InetAddress.h"
#include<memory>
#include<functional>
#include<sstream>
using std::shared_ptr;
using std::function;

class EventLoop;

class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
public:

    using TcpConnPtr=shared_ptr<TcpConnection>;
    using TcpConnCallBack=function<void (const TcpConnPtr&)>;

    TcpConnection(int fd,EventLoop *loop);//loop指针方便传EventLoop的this指针
    ~TcpConnection();
    void send(const string &msg);
    void sendInLoop(const string &msg);
    string receive();
    string toString(); 
    bool isClosed();

    void setConnectionCallback(const TcpConnCallBack &cb);
    void setMessageCallback(const TcpConnCallBack &cb);
    void setCloseCallback(const TcpConnCallBack &cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();


private:
    EventLoop* _loop;
    SocketIO _sockIO;
    Socket _sock;

    InetAddress _localAddr;
    InetAddress _peerAddr;
    InetAddress getPeerAddr();
    InetAddress getLocalAddr();

    TcpConnCallBack _onConn;
    TcpConnCallBack _onMsg;
    TcpConnCallBack _onClose;
};



#endif
