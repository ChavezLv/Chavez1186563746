#include "../../include/InetAddress.hpp"

#include <string.h>

namespace spellCorrect
{

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

}//end of namespace spellCorrect

