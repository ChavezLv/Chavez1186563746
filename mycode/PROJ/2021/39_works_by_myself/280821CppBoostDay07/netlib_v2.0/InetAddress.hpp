/*================================================================
*   
*   @file       ：InetAddress.hpp
*   @date       ：2021年08月29日 20:55:03
*   @description：
*
================================================================*/


#ifndef __INETADDRESS_HPP__
#define __INETADDRESS_HPP__

#include <string.h>// memset
#include <arpa/inet.h>// struct sockaddr_in
#include <string>
using std::string;

namespace wd{

class InetAddress{
public:
    InetAddress(unsigned short port, const string & ip){
        ::memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr(ip.c_str());
        _addr.sin_port = htons(port);

    }
    
    InetAddress(const struct sockaddr_in& addr)
    : _addr(addr){

    }

    string ip() const { return inet_ntoa(_addr.sin_addr); }
    unsigned int port() const { return ntohs(_addr.sin_port); }

    //虽然目前类sizeof(InetAddress) == sizeof(_addr), 
    //可以不需要以下这个, 比如bind(fd(), (struct sockaddr*)&_addr, \
                   sizeof(_addr)), 可以直接写_addr强转, 也是一样的效果
    struct sockaddr_in* getInetAddressPtr() { return &_addr; }

private:
    struct sockaddr_in _addr;

};

}//end of namespace wd

#endif //INETADDRESS_HPP
