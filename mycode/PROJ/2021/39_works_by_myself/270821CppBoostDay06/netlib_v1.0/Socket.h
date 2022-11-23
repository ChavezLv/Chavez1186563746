/**
 * Project netli_v1.0
 */


#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Noncopyable.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace wd{


class Socket: public Noncopyable {
public: 

    Socket(){
       _fd = ::socket(AF_INET, SOCK_STREAM, 0); 
       /* _fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); */ 
       if( _fd < 0 ){
           ::strerror(_fd);
       }
    }

    /**
     * @param int
     */
    explicit
    Socket(int fd)
    : _fd(fd){
                
    }

    ~Socket(){
        ::close(_fd);
    }

    int fd() const { return _fd; }

    void shutdownWrite();
    void setNonblock();

private: 
    int _fd;
};

}// end of namespace wd

#endif //_SOCKET_H
