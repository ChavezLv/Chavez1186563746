#ifndef __INETADDRESS_HPP__
#define __INETADDRESS_HPP__

#include <arpa/inet.h>

#include <string>

using std::string;

namespace spellCorrect
{

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

}//end of namespace spellCorrect

#endif
