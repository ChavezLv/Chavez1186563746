#ifndef __Acceptor_H__
#define __Acceptor_H__
#include"InetAddress.h"
#include"socket.h"
#include<sys/socket.h>
class Acceptor{
public:
    Acceptor (const string & ip,unsigned short port);
    ~Acceptor();
    void ready();
    int accept();
    int fd()const;
private:
    void setReusedAddr();
    void setReusedPort();
    void bind();
    void listen();

private:
    Socket _sockListen;    
    InetAddress _serverAddr;
};





#endif
