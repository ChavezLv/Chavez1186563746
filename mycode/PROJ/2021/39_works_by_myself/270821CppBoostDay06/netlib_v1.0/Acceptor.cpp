/**
 * Project netli_v1.0
 */


#include "Acceptor.h"

#include <iostream>
namespace wd{

/**
 * Acceptor implementation
 */

/**
 * @return void
 */
void Acceptor::ready() {
    setReuseAddr();
    setReusePort();
    bind();
    listen();
    return;
}


/**
 * @return int
 */
int Acceptor::accept() {
    int peerFd = ::accept(fd(), nullptr, nullptr);
    if(0 > peerFd){
        perror("accept");
    }
    return peerFd;
}


/**
 * @return void
 */
void Acceptor::bind() {
    /* if(0 != ::bind(fd(), (struct sockaddr*)_addr.getInetAddressPtr(), \ */
    if(0 != ::bind(fd(), (struct sockaddr*)&_addr, \
                   sizeof(_addr))){
        perror("bind");
    }
    std::cout << "sizeof(_addr)" << sizeof(_addr) << std::endl;
    std::cout << "sizeof" << sizeof(*_addr.getInetAddressPtr()) << std::endl;
    return;
}

/**
 * @return void : TIME_WAIT
 */
void Acceptor::setReuseAddr() {
    int optval = 1;
    if(0 != setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
        perror("setsockopt");
    }
    return;
}

/**
 * @return void
 */
void Acceptor::setReusePort() {
    int optval = 1;
    if(0 != setsockopt(fd(), SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval))){
        perror("setsockopt");
    }
    return;
}

/**
 * @return void
 */
void Acceptor::listen() {
    if(0 != ::listen(fd(), 128)){//$cat /proc/sys/net/core/somaxconn
        perror("listen");
    }

    return;
}

}//end of namespace wd
