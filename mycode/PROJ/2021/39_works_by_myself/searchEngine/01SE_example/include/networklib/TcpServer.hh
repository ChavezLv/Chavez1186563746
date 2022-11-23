/* <TcpServer> -*- C++ -*- */
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.hh"
#include "TcpConnection.hh"
#include "EventLoop.hh"

#include <string>

namespace wd
{

class TcpServer
{
public:
    TcpServer(const std::string & ip, unsigned short port);
    
    void start();
    void stop();

    void setConnectionCallback(Callback && cb);
    void setMessageCallback(Callback && cb);
    void setCloseCallback(Callback && cb);

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

} // end of namespace wd
#endif

