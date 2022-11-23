#ifndef __RXL_INETADDRESS_H__
#define __RXL_INETADDRESS_H__
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<string>
using std::string;
namespace rxl{
class InetAddress{
public:
	explicit
	InetAddress(unsigned short port){
		bzero(&_addr,sizeof(struct sockaddr_in));
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(port);
		_addr.sin_addr.s_addr = INADDR_ANY;
	}
	InetAddress(const string ip,unsigned short port){
		bzero(&_addr,sizeof(struct sockaddr_in));
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(port);
		_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	InetAddress(struct sockaddr_in& addr): _addr(addr){}
	string ip() const
	{
		return string(inet_ntoa(_addr.sin_addr));
	}
	short unsigned port() const
	{
		return ntohs(_addr.sin_port);
	}
	struct sockaddr* getSockAddressPtr(){
		return (struct sockaddr*)&_addr;
	}
private:
	struct sockaddr_in _addr;
};
}//end of namespace
#endif 
