/*================================================================
*   
*   @file       ：Socket.hpp
*   @date       ：2021年08月29日 20:34:48
*   @description：
*
================================================================*/


#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include "Noncopyable.hpp"

namespace wd{

class Socket
: public Noncopyable{

public:
    Socket();
    explicit Socket(int fd);//注意explicit
    int fd() const;

    void setNonblock();
    void shutdownWrite();

    ~Socket();

private:
    int _fd;
};


}//end of namespace wd

#endif //SOCKET_HPP
