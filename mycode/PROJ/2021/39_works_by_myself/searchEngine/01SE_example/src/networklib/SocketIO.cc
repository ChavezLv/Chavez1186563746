#include "../../libs/Mylog.hh"
#include "../../include/networklib/SocketIO.hh"

#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>


namespace wd
{

SocketIO::SocketIO(int fd)
    : _fd(fd)
{}

int SocketIO::readn(char * buff, int len)
{
    int ret, left = len;
    char * p = buff;
    while(left > 0) {
        ret = ::read(_fd, p, left);
        if(-1 == ret && errno == EINTR) {
            continue;
        } else if(-1 == ret) {
            logError("readn");
            return len - left;
        } else if(0 == ret) {
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::writen(const char * buff, int len)
{
    int ret, left = len;
    const char * p = buff;
    while(left > 0) {
        ret = ::write(_fd, p, left);
        if(ret == -1 && errno == EINTR) {
            continue;
        } else if(ret == -1) {
            logError("writen");
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::readline(char * buff, int maxlen)
{
    int ret, left = maxlen - 1, total = 0;
    char * p = buff;
    while(left > 0) {
        ret = recvPeek(p, left);
        for(int idx = 0; idx != ret; ++idx) {
            if(p[idx] == '\n') {
                readn(p, ++idx);
                total += idx;
                *(p + idx) = '\0';
                return total;
            }
        }
        // 如果没有找到 \n
        readn(p, ret);
        left -= ret;
        p += ret;
        total += ret;
    }
    *p = '\0';
    return total;
}


int SocketIO::recvPeek(char * buff, int len)
{
    int ret;
    do {
        ret = recv(_fd, buff, len, MSG_PEEK);
    }while(ret == -1 && errno == EINTR);
    return ret;
}


} // end of namespace wd
