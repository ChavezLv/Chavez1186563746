/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Socket.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 19:58:40
*   @description：
*
================================================================*/


#include "Socket.hpp"

#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

namespace wd{

Socket::Socket()
{
    _fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == _fd)
    {
        perror("socket");
    }
}

Socket::Socket(int fd)
    :_fd(fd)
{}

Socket::~Socket()
{
    ::close(_fd);
}

int Socket::fd() const
{
    return _fd;
}

void Socket::shutdownWrite()
{
    ::shutdown(_fd,SHUT_WR);
}

void Socket::setNonblock()
{
    int flag = ::fcntl(_fd,F_GETFL,0);
    if(flag < 0)
    {
        perror("fcntl_get");
        return;
    }
    flag |= O_NONBLOCK;
    int ret = ::fcntl(_fd,F_SETFL,flag);
    if(ret < 0)
    {
        perror("fcntl_set");
        return;
    }
    return;
}


}//end of namespace wd

