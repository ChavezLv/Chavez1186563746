/* <Socket> -*- C++ -*- */
#ifndef __WD_SOCKET_H__
#define __WD_SOCKET_H__

#include <sys/socket.h>

namespace wd
{

// 用于套接字的创建和销毁
class Socket
{
public:
    explicit
    Socket(int fd);
    Socket();
    ~Socket();

    int fd() const;
    void shutdownWrite();

private:
    int _fd;
};

} // end of namespace wd
#endif

