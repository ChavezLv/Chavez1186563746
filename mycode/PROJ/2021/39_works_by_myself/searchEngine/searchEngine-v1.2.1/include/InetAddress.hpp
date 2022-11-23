/*================================================================
*   
*   @file       ：InetAddress.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_INETADDRESS_HPP__
#define __WD_INETADDRESS_HPP__

#include <netinet/in.h>
#include <string>
using std::string;

namespace wd
{

class InetAddress
{
public:
	/* InetAddress(unsigned short port, const string & ip = "127.0.0.1"); */
	InetAddress(unsigned short port, const string & ip = "192.168.10.132");
	InetAddress(const struct sockaddr_in & addr);
	string ip() const;
	unsigned short port() const;

	struct sockaddr_in * getInetAddressPtr() {	return &_addr;	}

private:
	struct sockaddr_in _addr;
};

}//end of namespace wd
 
 
 
#endif
