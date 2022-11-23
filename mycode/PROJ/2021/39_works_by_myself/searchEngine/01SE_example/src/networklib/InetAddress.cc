#include "../../include/networklib/InetAddress.hh"

#include <string.h>

using std::string;

namespace wd
{

InetAddress::InetAddress(const std::string ip, unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
    _addr.sin_port = ::htons(port);
}

InetAddress::InetAddress(unsigned short port)
{
    ::memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY; // 本地IP地址
    _addr.sin_port = ::htons(port);
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
    : _addr(addr)
{}

unsigned short InetAddress::port() const
{
    return ::ntohs(_addr.sin_port);
}

std::string InetAddress::ip() const
{
    return string(::inet_ntoa(_addr.sin_addr));
}

struct sockaddr * InetAddress::getInetAddressPtr()
{
    return (struct sockaddr*)&_addr;
}

} // end of namespace wd
