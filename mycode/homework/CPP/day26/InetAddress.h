#ifndef __InetAddress_H__
#define __InetAddress_H__
#include<arpa/inet.h>
#include<string>
using std::string;
class InetAddress{
public:
    InetAddress(const string &ip,unsigned short port);
    InetAddress(const struct sockaddr_in&);
    ~InetAddress();
    string ip();
    unsigned short port();
    sockaddr_in* getInetAddPtr();
private:
    struct sockaddr_in _seraddr;
};





#endif
