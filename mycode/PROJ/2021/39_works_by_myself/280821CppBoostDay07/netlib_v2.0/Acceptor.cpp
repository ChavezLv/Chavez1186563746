/*================================================================
*   
*   @file       ：Acceptor.cpp
*   @date       ：2021年08月29日 21:17:04
*   @description：
*
================================================================*/


#include "Acceptor.hpp"

namespace wd{

void Acceptor::setReuseAddr(){
    int optval = 1;
    socklen_t len = sizeof(optval);
    if(0 != setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, &optval, len)){ 
        perror("setsockopt");
    }
}

void Acceptor::setReusePort(){
    int optval = 1;
    socklen_t len = sizeof(optval);
    if(0 != setsockopt(fd(), SOL_SOCKET, SO_REUSEPORT, &optval, len)){ 
        perror("setsockopt");
    }
}

void Acceptor::bind(){
    //目前类sizeof(InetAddress) == sizeof(_addr), 
    //可以不需要以下这个, 比如bind(fd(), (struct sockaddr*)&_addr, 
                   //sizeof(_addr)), 可以直接写_addr强转, 也是一样的效果
    if(0 != ::bind(fd(), (struct sockaddr*)_addr.getInetAddressPtr(),
                   sizeof(_addr))){
        perror("bind");
    }
}

void Acceptor::listen(){
    int somaxconn = 128;// cat /proc/sys/net/core/somaxconn
    if(0 != ::listen(fd(), somaxconn)){
        perror("listen");
    }
}

int Acceptor::accept(){
    int peerFd = ::accept(fd(), nullptr, nullptr);
    if(0 > peerFd){
        strerror(peerFd);
    }
    return peerFd;
}

}//end of namespace wd

