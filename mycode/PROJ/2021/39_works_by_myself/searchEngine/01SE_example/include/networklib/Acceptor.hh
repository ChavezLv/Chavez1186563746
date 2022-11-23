/* <Acceptor> -*- C++ -*- */
#ifndef __WD_ACCEPTOR_H__
#define __WD_ACCEPTOR_H__

#include "InetAddress.hh"
#include "Socket.hh"

namespace wd
{

class Acceptor
{
public:
    explicit
    Acceptor(unsigned short port);
    Acceptor(const std::string & ip, unsigned short port);

    void ready();
    int accept();

    int fd() const { return _listenSock.fd(); }

private:
    void setReuseAddr(bool);
    void setReusePort(bool);
    void bind();
    void listen();

private:
    Socket _listenSock;
    InetAddress _addr;
};

} // end of namespace wd
#endif

