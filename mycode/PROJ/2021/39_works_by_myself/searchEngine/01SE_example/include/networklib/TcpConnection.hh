/* <TcpConnection> -*- C++ -*- */
#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.hh"
#include "SocketIO.hh"
#include "InetAddress.hh"
#include "Noncopyable.hh"

#include <functional>
#include <memory>

namespace wd
{

class EventLoop;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using Callback = std::function<void(const TcpConnectionPtr&)>;

class TcpConnection
    : Noncopyable
    , public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd, EventLoop * loop);

    std::string receive(); // 接收数据
    void send(const std::string & msg); // 发送数据
    void sendInLoop(const std::string & msg);

    int fd() const;

    std::string toString() const;
    void shutdown();

    void setConnectionCallback(const Callback &);
    void setMessageCallback(const Callback &);
    void setCloseCallback(const Callback &);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr; // 源IP、地址
    InetAddress _peerAddr; // 目的IP、地址
    // 传参为this
    Callback _onConnection;
    Callback _onMessage;
    Callback _onClose;
    bool _isShutdownWrite;

    EventLoop * _loop;
};

}
#endif

