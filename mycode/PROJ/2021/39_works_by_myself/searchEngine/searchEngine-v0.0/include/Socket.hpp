/*================================================================
*   
*   @file       ：Socket.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_SOCKET_HPP__
#define __WD_SOCKET_HPP__

#include "Noncopyable.hpp"

namespace wd
{

class Socket : Noncopyable
{
public:
	Socket();
	explicit Socket(int fd);
	~Socket();

	int fd() const {	return _fd;	}
	//主动关闭连接的写端时，可以调用
	void shutdownWrite();
	void setNonblock();

private:
	int _fd;
};

}//end of namespace wd
 
 
 
#endif
