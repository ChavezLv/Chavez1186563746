/**
 * Project netli_v1.0
 */


#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "InetAddress.h"
#include "Socket.h"

namespace wd{


class Acceptor {
public: 
    
    /**
     * @param port
     * @param ip
     */
    /* Acceptor(const unsigned short &port, const string &ip = "127.0.0.1") */
    Acceptor(const unsigned short &port, const string &ip = "192.168.10.132")
    : _sockListen()
    , _addr(port, ip){

    }

    int accept();

    void ready();

    int fd() const { return _sockListen.fd(); }
private:
    void bind();

    void setReuseAddr();

    void setReusePort();

    void listen();
    
private: 
    Socket _sockListen;
    InetAddress _addr;

};

}//end of namespace wd

#endif //_ACCEPTOR_H
