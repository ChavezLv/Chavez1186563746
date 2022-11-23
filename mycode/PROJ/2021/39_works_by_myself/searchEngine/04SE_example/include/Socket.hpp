#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

namespace spellCorrect
{

class Socket
{
public:
    Socket();
    explicit
        Socket(int fd);
    ~Socket();

    int fd() const;
    void shutdownWrite();
    void setNonblock();
private:
    int _fd;
};



}//end of namespace spellCorrect

#endif
