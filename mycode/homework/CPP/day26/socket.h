#ifndef __socket_H__
#define __socket_H__

class Socket{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd()const;
    void shutDownWrite();
private:
    int _fd;
};




#endif
