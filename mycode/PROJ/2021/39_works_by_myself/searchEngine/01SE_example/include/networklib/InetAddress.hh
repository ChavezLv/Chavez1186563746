/* <InetAddress> -*- C++ -*- */
#ifndef __WD_INETADDRESS_H__
#define __WD_INETADDRESS_H__

#include <arpa/inet.h>

#include <string>

namespace wd
{

class InetAddress
{
public:
    explicit
    InetAddress(unsigned short port);
    InetAddress(const std::string ip, unsigned short port);
    InetAddress(const struct sockaddr_in & addr);

    unsigned short port() const;
    std::string ip() const;

    struct sockaddr * getInetAddressPtr();

private:
    struct sockaddr_in _addr;
};

} // end of namespace wd
#endif

