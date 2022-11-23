/**
 * Project netli_v1.0
 */


#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

namespace  wd{


class TcpConnection: public Noncopyable {
public: 
    TcpConnection(int fd)
    : _sock(fd)
    , _sockIO(fd)
    , _localAddr(getLocalAddr())
    , _peerAddr(getPeerAddr())
    , _shutdownWrite(false){

    }
    ~TcpConnection() {  }

    string receive();

    /**
     * @param const string & msg
     */
    void send(const string & msg);

    string toString() const;
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr(); 
private: 
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _shutdownWrite;
};

}//end of namespace wd

#endif //_TCPCONNECTION_H
