#ifndef __WD_SOCKET_HPP__
#define __WD_SOCKET_HPP__

namespace wd
{

class Socket
{
public:
    Socket(); 
    Socket(int);
    ~Socket(); 
    void shutdownWrite();
    int fd() const;

private:
    int _fd;
};

}//end of namespace wd

#endif
