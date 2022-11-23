/*================================================================
*   
*   @file       ：Socket.cpp
*   @date       ：2021年08月29日 20:39:36
*   @description：
*
================================================================*/


#include "Socket.hpp"
#include <stdio.h>//perror
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>// strerror
#include <unistd.h>// close

namespace wd{

Socket::Socket(){
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if( _fd < 0 ){
        ::strerror(_fd);
    }
}

Socket::Socket(int fd){
    _fd = fd; 
}

void Socket::setNonblock(){
    int statusFlag = ::fcntl(_fd, F_GETFL, 0);
    ::fcntl(_fd, F_SETFL, statusFlag | O_NONBLOCK);

}

void Socket::shutdownWrite(){
    if(0 != ::shutdown(_fd, SHUT_WR)){
        perror("shutdown");
    }
}

int  Socket::fd() const {
    return _fd;
}

Socket::~Socket(){
    if(0 != ::close(_fd)){
        perror("close");
    }
}

}//end of namespace wd

