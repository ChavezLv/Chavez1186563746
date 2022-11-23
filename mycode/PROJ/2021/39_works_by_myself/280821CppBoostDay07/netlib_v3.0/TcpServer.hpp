/*================================================================
*   
*   @file       ：TcpServer.hpp
*   @date       ：2021年08月30日 10:39:17
*   @description：
*
================================================================*/


#ifndef __WD_TCPSERVER_HPP__
#define __WD_TCPSERVER_HPP__

#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "TcpConnection.hpp"

namespace wd{

class TcpServer{
public:
    TcpServer(unsigned short port, const string & ip = "192.168.10.132")
    : _acceptor(port, ip)
    , _loop(_acceptor){

    }

    void start(){
        _acceptor.ready();
        _loop.loop();
    }
    void stop() { _loop.unloop(); }

    void setAllCallbacks(TcpConnectionCallback && cb1,
			TcpConnectionCallback && cb2,
            TcpConnectionCallback && cb3){
        _loop.setAllCallbacks(std::move(cb1),\
                              std::move(cb2),\
                              std::move(cb3));
    }

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

}//end of namespace wd

#endif //__WD_TCPSERVER_HPP__
