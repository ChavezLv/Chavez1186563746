/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：InetAddress.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 20:11:08
*   @description：
*
================================================================*/


#include "InetAddress.hpp"
#include <string.h>

namespace wd{

InetAddress::InetAddress(unsigned short port)
{
    ::memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0");本机
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const string& ip, unsigned short port)
{
    ::memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
    :_addr(addr)
{}

string InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAddress::getInetAddressPtr()
{
    return &_addr;
}

}//end of namespace wd

