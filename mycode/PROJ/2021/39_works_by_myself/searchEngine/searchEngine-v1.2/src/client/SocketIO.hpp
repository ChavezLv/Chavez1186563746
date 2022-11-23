/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：SocketIO.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 19:59:50
*   @description：
*
================================================================*/


#ifndef __WD_SOCKETIO_HPP__
#define __WD_SOCKETIO_HPP__

namespace wd{

class SocketIO
{
public:
    explicit
        SocketIO(int fd);

    int readn(void* buf, int len);
    int readTrain(char* buf);

    int writen(const void* buf, int len);
    int writeTrain(const char* buf,int dataLen);

private:
    int _fd;
};

}//end of namespace wd

#endif //__WD_SOCKETIO_HPP__
