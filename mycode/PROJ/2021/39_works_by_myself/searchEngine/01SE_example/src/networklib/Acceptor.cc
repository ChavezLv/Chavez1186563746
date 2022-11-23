#include "../../libs/Mylog.hh"
#include "../../include/networklib/Acceptor.hh"
#include <sys/socket.h>

namespace wd
{

Acceptor::Acceptor(unsigned short port)
    : _listenSock()
    , _addr(port)
{}

Acceptor::Acceptor(const std::string & ip, unsigned short port)
    : _listenSock()
    , _addr(ip, port)
{}


void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}


void Acceptor::setReuseAddr(bool flag)
{
    int f = flag;
    if(-1 == setsockopt(
            _listenSock.fd(),
            SOL_SOCKET,
            SO_REUSEADDR,
            &f, sizeof(f))) {
        logError("setReuseAddr");
    }
}

void Acceptor::setReusePort(bool flag)
{
    int f = flag;
    if(-1 == setsockopt(
            _listenSock.fd(),
            SOL_SOCKET,
            SO_REUSEPORT,
            &f, sizeof(f))) {
        logError("setReusePort");
    }
}

void Acceptor::bind()
{
    if(-1 == ::bind(
            _listenSock.fd(),
            (struct sockaddr*)_addr.getInetAddressPtr(),
            sizeof(struct sockaddr))) {
        logError("bind");
    }
}

void Acceptor::listen()
{
    if(-1 == ::listen(
            _listenSock.fd(), 10)) {
        logError("listen");
    }
}

int Acceptor::accept()
{
    int fd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if(fd == -1) {
        logError("accept");
    }
    return fd;
}

}// end of namespace wd
