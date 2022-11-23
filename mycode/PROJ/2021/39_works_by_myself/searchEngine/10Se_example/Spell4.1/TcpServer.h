#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "Acceptor.h"
#include "EventLoop.hpp"
#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
using std::function;
namespace wd{
    using TcpConnectionPtr=shared_ptr<TcpConnection>;
    using TcpCallBack=function<void(const TcpConnectionPtr&)>;

class TcpServer
{
public:

    TcpServer(const string&ip,unsigned int port) 
    :_accept(ip,port)
    ,_event(_accept)
    {

    }
    void setNewConnectionCallBack(TcpCallBack&&cb){
        _event.setNewConnectionCallBack(std::move(cb));
    }
    void setMessageCallBack(TcpCallBack&&cb){
        _event.setMsgCallBack(std::move(cb));
    }
    void setCloseCallBack(TcpCallBack&&cb){
        _event.setCloseCallBack(std::move(cb));
    }
    void start(){
        _accept.ready();
        _event.loop();
    }
    ~TcpServer() {}

private:
    Acceptor _accept;
    EventLoop _event;
};

}//end of namespace;
#endif
