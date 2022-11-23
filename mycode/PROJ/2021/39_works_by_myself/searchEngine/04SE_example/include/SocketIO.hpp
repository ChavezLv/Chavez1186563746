#ifndef __SOCKETIO_HPP__
#define __SOCKETIO_HPP__

namespace spellCorrect
{

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

}//end of namespace spellCorrect

#endif
