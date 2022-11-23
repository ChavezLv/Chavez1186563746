#ifndef __RXL_SOCKETIO_H__
#define __RXL_SOCKETIO_H__
namespace rxl{
class SocketIO{
public:
	SocketIO(int fd):_fd(fd){}
	int readn(char *buf,int len);
	int readline(char *buf,int maxlen);
	int writen(const char* buf,int len);
	int writen_train(const char* buf,int len);
private:
	int recvPeek(char *buf,int len);
private:
	int _fd;
};
}//end of namespace
#endif 
