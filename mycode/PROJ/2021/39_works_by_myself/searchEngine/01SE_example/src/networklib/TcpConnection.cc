#include "../../libs/Mylog.hh"
#include "../../include/networklib/TcpConnection.hh"
#include "../../include/networklib/EventLoop.hh"

#include <features.h>
#include <memory>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>

namespace wd
{

TcpConnection::TcpConnection(int fd, EventLoop * loop)
    : _sock(fd)
    , _sockIO(fd)
    , _localAddr(getLocalAddr())
    , _peerAddr(getPeerAddr())
    , _isShutdownWrite(false)
    , _loop(loop)
{}


std::string TcpConnection::receive()
{
    char buff[65536] = {0};
    size_t len;
    _sockIO.readn((char*)&len, sizeof(len));
    _sockIO.readn(buff, len);
    return std::string(buff);
}

void TcpConnection::send(const std::string & msg)
{
    size_t len = msg.size();
    _sockIO.writen((char*)&len, sizeof(size_t));
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const std::string & msg)
{
    _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
}

int TcpConnection::fd() const
{
    return _sock.fd();
}

std::string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << "S " << _localAddr.ip() << ":" << _localAddr.port() << " <--> "
        << "C " << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}

void TcpConnection::shutdown()
{
    if(!_isShutdownWrite) {
        _isShutdownWrite = true;
        _sock.shutdownWrite();
    }
}

InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getsockname(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
        logError("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getpeername(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
        logError("getpeername");
    }
    return InetAddress(addr);
}

void TcpConnection::setConnectionCallback(const Callback & cb)
{
    _onConnection = std::move(cb);
}

void TcpConnection::setMessageCallback(const Callback & cb)
{
    _onMessage = std::move(cb);
}

void TcpConnection::setCloseCallback(const Callback & cb)
{
    _onClose = std::move(cb);
}


void TcpConnection::handleConnectionCallback()
{
    if(_onConnection) {
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessage) {
        _onMessage(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onClose) {
        _onClose(shared_from_this());
    }
}


} // end of namespace wd

