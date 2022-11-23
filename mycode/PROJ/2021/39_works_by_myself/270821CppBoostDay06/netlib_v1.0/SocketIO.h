/*================================================================
*   
*   @file       ：SocketIO.h
*   @date       ：2021年08月27日 22:08:30
*   @description：
*
================================================================*/

#ifndef __WD_SOCKETIO_H__
#define __WD_SOCKETIO_H__ 

namespace wd{

class SocketIO{
public:
    SocketIO(int fd)
    : _fd(fd){

    }

    int readn(char* buf, int len);
    int readline(char* buf, int maxlen);
    int writen(const char* buf, int len);
private:
    int _fd;
};

}//end of namespace wd

#endif

