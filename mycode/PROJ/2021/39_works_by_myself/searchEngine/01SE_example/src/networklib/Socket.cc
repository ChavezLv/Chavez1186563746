#include "../../libs/Mylog.hh"
#include "../../include/networklib/Socket.hh"

#include <unistd.h>
#include <sys/socket.h>
#include <cstdio>

namespace wd
{

Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd == -1) {
        logError("get socketFd");
    }
}

Socket::Socket(int fd) : _fd(fd) {}

Socket::~Socket()
{
    ::close(_fd);
}

int Socket::fd() const { return _fd; }

void Socket::shutdownWrite()
{
    // 停止发信息
    ::shutdown(_fd, SHUT_WR);
}

}

