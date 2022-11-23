#ifndef __TcpConn_H__
#define __TcpConn_H__
#include"socket.cpp"
#include"SockerIO.cpp"
#include"InetAddress.h"
class TcpConn{
public:
    TcpConn(int fd);
    ~TcpConn();
    void send(const string &msg);
    string receive();
    string toString();
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SockerIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
};




#endif
