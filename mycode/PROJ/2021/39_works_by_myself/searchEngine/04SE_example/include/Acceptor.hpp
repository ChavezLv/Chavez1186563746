#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__

#include "Socket.hpp"
#include "InetAddress.hpp"

namespace spellCorrect
{

class Acceptor
{
public:
    Acceptor(unsigned short port);
    Acceptor(const string& ip,unsigned short port);

    void ready();
    int accept();

    int fd() const;//listenFd
private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();
private:
    Socket _listenSocket;
    InetAddress _addr;
};

}//end of namespace spellCorrect

#endif
