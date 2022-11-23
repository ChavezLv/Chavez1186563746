#ifndef __o2multi_H__
#define __o2multi_H__
#include<sys/socket.h>
#include<memory.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string>
using std::string;
class Server{
public:
    virtual void acceptor()=0;
    virtual void sendm()=0;
    virtual void loop()=0;
};
using Sockaddr=struct sockaddr_in;
class Socket
:public Server
{
public:
    Socket(const string &ip,unsigned short port);
    Socket(sockaddr_in& servAddr);
    void initial();
    void acceptor() override;
    void sendm()override;
    void virtual loop()override;
    ~Socket();
private:
    string _ip;
    unsigned short _port;
    Sockaddr _servAddr;
    int _sockFd;
    int _newFd;
};




#endif
