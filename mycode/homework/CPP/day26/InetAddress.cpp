#include"InetAddress.h"
#include<memory.h>
InetAddress::InetAddress(const string &ip,unsigned short port)
{
    ::bzero(&_seraddr,sizeof(struct sockaddr_in));
    _seraddr.sin_family=AF_INET;
    _seraddr.sin_addr.s_addr=inet_addr(ip.c_str());
    _seraddr.sin_port=htons(port);
}
InetAddress::InetAddress(const struct sockaddr_in&)
    :_seraddr(){}

unsigned short InetAddress::port(){
    return ntohs(_seraddr.sin_port);
}
string InetAddress::ip(){
    return string(inet_ntoa(_seraddr.sin_addr));
}
sockaddr_in*InetAddress:: getInetAddPtr(){
    return &_seraddr;
}
InetAddress::~InetAddress(){}
