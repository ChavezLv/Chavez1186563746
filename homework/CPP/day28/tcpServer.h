#ifndef __tcpServ_H__
#define __tcpServ_H__

#include"Acceptor.h"
#include"eventLoop.h"

class TcpServer{
public:
    TcpServer(const string& ip,unsigned short port)
        :_acceptor(ip,port)
        ,_loop(_acceptor){}
    ~TcpServer(){}

    void start(){
        _acceptor.ready();
        _loop.loop();
    }
    void stop(){
        _loop.unloop();
    }

    void setAllCallback(TcpConnectionCallback&& onConn, \
                        TcpConnectionCallback&& onMsg, \
                        TcpConnectionCallback&& onClose){
            _loop.setConnectionCallback(std::move(onConn));
            _loop.setMessageCallback(std::move(onMsg));
            _loop.setCloseCallback(std::move(onClose));
    }
private:
    Acceptor _acceptor;
    EventLoop _loop;
};




#endif
