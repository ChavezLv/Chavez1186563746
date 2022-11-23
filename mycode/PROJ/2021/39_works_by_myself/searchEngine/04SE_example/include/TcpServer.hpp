#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "TcpConnection.hpp"

namespace spellCorrect
{

class TcpServer
{
public:
    TcpServer(const string& ip,unsigned short port);

    void setConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback && cb);
    void setCloseCallback(TcpConnectionCallback && cb);

    void start();
    void stop();//由其他进程执行，start是死循环
private:
    Acceptor _acceptor;
    EventLoop _loop;
};


}//end of spellCorrect

#endif
