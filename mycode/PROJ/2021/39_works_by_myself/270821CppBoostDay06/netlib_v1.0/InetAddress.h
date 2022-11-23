/**
 * Project netli_v1.0
 */


#ifndef __WD_INETADDRESSS_H__
#define __WD_INETADDRESSS_H__

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
using std::string;


using InetAddressPtr =  struct sockaddr_in *;

namespace wd{


class InetAddress {
public: 

    /**
     * @param port
     * @param ip
     */
    InetAddress(unsigned short port, const string & ip){
        ::memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET; 
        _addr.sin_port = htons(port);
        _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    InetAddress(const struct sockaddr_in& addr)
    : _addr(addr){

    }

    string ip() const;

    unsigned short port() const;

    InetAddressPtr getInetAddressPtr(){ return &_addr; }

private: 
    struct sockaddr_in _addr;
};


}// end of namespace wd

#endif //_INETADDRESS_H
