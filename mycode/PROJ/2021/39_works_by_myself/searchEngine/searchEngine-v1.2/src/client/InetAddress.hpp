/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：InetAddress.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 20:08:47
*   @description：
*
================================================================*/

#include <arpa/inet.h>

#include <string>

using std::string;


#ifndef __WD_INETADDRESS_HPP__
#define __WD_INETADDRESS_HPP__

namespace wd{

class InetAddress
{
public:
    explicit
        InetAddress(unsigned short port);
    InetAddress(const string& ip, unsigned short port);
    InetAddress(const struct sockaddr_in& addr);

    string ip() const;
    unsigned short port() const;
    struct sockaddr_in* getInetAddressPtr();
private:
    struct sockaddr_in _addr;
};

}//end of namespace wd

#endif //__WD_INETADDRESS_HPP__
