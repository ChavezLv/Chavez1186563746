#ifndef __TCPCONNECTION_HPP__
#define __TCPCONNECTION_HPP__

#include "NonCopyable.hpp"
#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"

#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

namespace spellCorrect
{
class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr& conn)>;

class TcpConnection
    :NonCopyable
     ,public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd,EventLoop* loop);//accept得到的peerfd，在TCP连接时表示本端
    ~TcpConnection();

    string receive();
    void send(const string& msg);

    string toString() const;
    void shutdown();

    void setConnectionCallback(const TcpConnectionCallback& cb);
    void setMessageCallback(const TcpConnectionCallback& cb);
    void setCloseCallback(const TcpConnectionCallback& cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

    bool isClosed();//确认连接是否关闭
    void setNonblock();//设置套接字为非阻塞

    void sendInLoop(const string& msg);//计算结果由EventLoop进行发送
private:
    InetAddress getLocalAddr(int fd);
    InetAddress getPeerAddr(int fd);
private:
    Socket _sock;
    SocketIO _socketIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite;
    
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;//将任务挂上任务队列
    TcpConnectionCallback _onClose;

    EventLoop* _loop;
};

}//end of namespace spellCorrect

#endif
