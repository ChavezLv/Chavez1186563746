#ifndef __RXL_SOKCET_H__
#define __RXL_SOKCET_H__
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
namespace rxl{
class Socket{
public:
	Socket(){
		_fd = ::socket(AF_INET,SOCK_STREAM,0);
		if(_fd == -1){
			perror("socket");
		}
	}
	explicit
	Socket(int fd):_fd(fd){}
	void shutdownWrite(){
		::shutdown(_fd,SHUT_WR);
	}
	int fd()const{return _fd;}
	~Socket(){::close(_fd);}
private:
	int _fd;
};
}//end of namespace

#endif 
