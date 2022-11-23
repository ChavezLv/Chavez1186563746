/*================================================================
*   
*   @file       ：SocketIO.hpp
*   @date       ：2021年08月29日 21:57:57
*   @description：
*
================================================================*/


#ifndef __SOCKETIO_HPP__
#define __SOCKETIO_HPP__

#include <stdio.h>// size_t
namespace wd{

//主要负责数据的输入输出
class SocketIO{
public:
    SocketIO(int fd)
    : _fd(fd){
        
    }

    int  readn(char* buf, size_t len);
    int readline(char *buf, size_t len);

    int writen(const char *buf, size_t len);

private:
    int _fd;

};

}//end of namespace wd

#endif //SOCKETIO_HPP
