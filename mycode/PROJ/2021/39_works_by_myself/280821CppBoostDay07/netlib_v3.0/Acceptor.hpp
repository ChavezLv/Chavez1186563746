/*================================================================
*   
*   @file       ：Acceptor.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/


#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__

#include "Socket.hpp"
#include "InetAddress.hpp"

namespace wd{

class Acceptor{
public:
    Acceptor(const unsigned short & port, const string &ip = "192.168.10.132")
    : _socket()
    , _addr(port, ip){

    }

    int fd() const { return _socket.fd(); }

    void ready(){
        setReuseAddr();
        setReusePort();
        bind();
        listen();
    }


    int accept();
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

private:
    Socket _socket;
    InetAddress _addr;
};

}//end of namespace wd

#endif //ACCEPTOR_HPP
