#include "../../include/SocketIO.hpp"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

/* SocketIO::SocketIO(int fd) */
/*     :_fd(fd) */
/* {} */

int SocketIO::readn(void* buf, int len)
{
    int left = len;
    char* p = (char*)buf;
    while(left > 0)
    {
        int ret = ::read(_fd,p,left);
        if(-1 == ret && errno == EINTR)
        {
            perror("read");
            continue;
        }
        else if(-1 == ret)
        {
            perror("read");
            break;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::readTrain(char* buf)
{
    int dataLen;
    while(1)
    {
        //接收长度
        readn(&dataLen,sizeof(dataLen));
        cout << "read datalen: " << dataLen << endl;
        if(0 == dataLen)
        {
            break;
        }
        //接收内容
        readn(buf,dataLen);
    }
    return dataLen + sizeof(dataLen);
}

int SocketIO::writen(const void* buf,int len)
{
    int left = len;
    const char* p = (const char*)buf;
    while(left > 0)
    {
        int ret = ::write(_fd, p, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("write");
            break;
        }
        else
        {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::writeTrain(const char* buf,int dataLen)
{
    //数据长度
    writen(&dataLen,sizeof(dataLen));
    cout << "write datalen: " << dataLen << endl;
    //文件内容
    writen(buf,dataLen);
    //结束标识
    dataLen = 0;
    writen(&dataLen,sizeof(dataLen));

    return dataLen + sizeof(dataLen);
}

}//end of namespace spellCorrect

