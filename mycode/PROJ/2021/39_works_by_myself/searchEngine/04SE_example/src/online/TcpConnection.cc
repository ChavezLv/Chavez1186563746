#include "../../include/TcpConnection.hpp"

#include "../../include/EventLoop.hpp"

#include <stdlib.h>
#include <sys/socket.h>

#include <sstream>
#include <iostream>

using std::cout;
using std::endl;

namespace spellCorrect
{

TcpConnection::TcpConnection(int fd, EventLoop* loop)
    :_sock(fd)
     ,_socketIO(fd)
     ,_localAddr(getLocalAddr(fd))
     ,_peerAddr(getPeerAddr(fd))
     ,_isShutdownWrite(false)
     ,_loop(loop)
    {}

TcpConnection::~TcpConnection()
{
    shutdown();
}

string TcpConnection::receive()
{
    char buf[65536] = {0};
    _socketIO.readTrain(buf);
    return string(buf);
}

void TcpConnection::send(const string& msg)
{
    _socketIO.writeTrain(msg.c_str(),msg.size());
}

string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() << " --> "
        << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}

void TcpConnection::shutdown()
{
    if(!_isShutdownWrite)
    {
        _sock.shutdownWrite();
    }
}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback& cb)
{
    _onConnection = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback& cb)
{
    _onMessage = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback& cb)
{
    _onClose = cb;
}

void TcpConnection::handleConnectionCallback()
{
    if(_onConnection)
    {
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessage)
    {
        _onMessage(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}

bool TcpConnection::isClosed()
{
    int ret = -1;
    char buf[1024] = {0};
    do
    {
        ret = recv(_sock.fd(),buf,sizeof(buf),MSG_PEEK);
    }while(-1 == ret && errno == EINTR);
    //用于onMessage
    //当tcp管道有数据时，或对端关闭时，才会触发epoll，因此可以验证连接是否关闭
    return 0 == ret;
}

void TcpConnection::setNonblock()
{
    _sock.setNonblock();
}

void TcpConnection::sendInLoop(const string& msg)
{
    if(_loop)
    {
        _loop->addFunctor(std::bind(&TcpConnection::send,this,msg));
    }
}

InetAddress TcpConnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(-1 == getsockname(fd,(struct sockaddr*)&addr,&len))
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(-1 == getpeername(fd,(struct sockaddr*)&addr,&len))
    {
        perror("getpeername");
    }
    return InetAddress(addr);
}

}//end of namespace spellCorrect

