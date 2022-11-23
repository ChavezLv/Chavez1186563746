/*================================================================
*   
*   @file       ：Connect.h
*   @date       ：2021年08月27日 23:15:54
*   @description：
*
================================================================*/

#ifndef __CONNECT_H__
#define __CONNECT_H__
#include "../Socket.h"
#include "../SocketIO.h"
#include "../InetAddress.h" 

namespace wd{

class Connect{

public:

    /* Connect(const unsigned short &port, const string &ip = " 127.0.0.1") */
    Connect(const unsigned short &port, const string &ip = "192.168.10.132")
    : _sock()
    , _sockIO(_sock.fd())
    , _addr(port, ip){

    }

    int ready(){
        socklen_t len = sizeof(_addr);
        if(0 > connect(_sock.fd(), (struct sockaddr *)&_addr, len)){
            close(_sock.fd());
            perror("connect");
            return -1;
        }
        return _sock.fd();

    }

    int send(const string &msg){
        return ::send(_sock.fd(), msg.data(), msg.size(), 0);
    }

    string recv(){
        char buf[2<<15] = { 0 };
        int ret = ::recv(_sock.fd(), buf, sizeof(buf) - 1, 0);
        return buf;    

    }


private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _addr;
};


}//end of namespace wd

#endif
