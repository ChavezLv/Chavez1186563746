/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Socket.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 19:57:55
*   @description：
*
================================================================*/


#ifndef __WD_SOCKET_HPP__
#define __WD_SOCKET_HPP__

namespace wd{

class Socket
{
public:
    Socket();
    explicit
    Socket(int fd);
    ~Socket();

    int fd() const;
    void shutdownWrite();
    void setNonblock();
private:
    int _fd;
};

}//end of namespace wd

#endif //__WD_SOCKET_HPP__
