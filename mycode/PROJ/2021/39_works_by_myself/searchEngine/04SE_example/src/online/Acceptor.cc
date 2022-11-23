#include "../../include/Acceptor.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace spellCorrect
{
Acceptor::Acceptor(unsigned short port)
    :_addr(port)
     ,_listenSocket()
    {}

Acceptor::Acceptor(const string& ip,unsigned short port)
    :_addr(ip,port)
     ,_listenSocket()
    {}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

int Acceptor::accept()
{
    int peerfd = ::accept(_listenSocket.fd(),nullptr,nullptr);
    if(-1 == peerfd)
    {
        perror("accept");
    }
    return peerfd;
}

int Acceptor::fd() const
{
    return _listenSocket.fd();
}

void Acceptor::setReuseAddr(bool on)
{
    int reuse = on;
    if(setsockopt(_listenSocket.fd()
                  ,SOL_SOCKET
                  ,SO_REUSEADDR
                  ,&reuse
                  ,sizeof(reuse)) < 0)
    {
        perror("setsockopt");
    }
}

void Acceptor::setReusePort(bool on)
{
    int reuse = on;
    if(setsockopt(_listenSocket.fd()
                  ,SOL_SOCKET
                  ,SO_REUSEPORT
                  ,&reuse
                  ,sizeof(reuse)) < 0)
    {
        perror("setsockopt");
    }
}

void Acceptor::bind()
{
    int ret = ::bind(_listenSocket.fd(),(struct sockaddr*)_addr.getInetAddressPtr(),sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSocket.fd(),10);
    if(-1 == ret)
    {
        perror("listen");
    }
}

}//end of namespace spellCorrect

