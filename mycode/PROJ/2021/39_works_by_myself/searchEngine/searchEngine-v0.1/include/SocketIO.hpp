/*================================================================
*   
*   @file       ：SocketIO.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_SOCKETIO_HPP__
#define __WD_SOCKETIO_HPP__

namespace wd
{

class SocketIO
{
public:
	SocketIO(int fd)
	: _fd(fd)
	{}

	int readn(char * buff, int len);
	int readline(char * buff, int maxlen);
	int writen(const char * buff, int len);

private:
	int _fd;
};

}//end of namespace wd
 
 
 
#endif
