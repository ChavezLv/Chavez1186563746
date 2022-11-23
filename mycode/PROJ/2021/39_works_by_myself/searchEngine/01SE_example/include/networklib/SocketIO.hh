/* <SocketIO> -*- C++ -*- */
#ifndef __WD_SOCKETIO_H__
#define __WD_SOCKETIO_H__

namespace wd
{

class SocketIO
{
public:
    explicit
    SocketIO(int fd);

    int readn(char * buff, int len);
    int writen(const char * buff, int len);
    int readline(char * buff, int maxlen);
    
    /* int readInt32(); */
    /* int writeInt32(); */

private:
    int recvPeek(char * buff, int len);

private:
    int _fd;

};

} // end of namespace wd
#endif

