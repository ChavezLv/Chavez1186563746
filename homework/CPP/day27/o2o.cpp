#include<sys/socket.h>
#include<memory.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string>
#include<iostream>
using std::cout;
using std::endl;
using std::string;
/* 1,socket
 * 2,bind
 * 3,listen
 * 4,accept
 * 5,send/recv*/
class Server{
public:
    virtual void acceptor()=0;
    virtual void sendm()=0;
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
    ~Socket();
private:
    string _ip;
    unsigned short _port;
    Sockaddr _servAddr;
    int _sockFd;
    int _newFd;
};
Socket::~Socket(){
    close(_sockFd);
}
void Socket::acceptor(){
    bind(_sockFd,(struct sockaddr*)&_servAddr,sizeof(struct sockaddr_in));
    listen(_sockFd,1);//one to one;
    socklen_t len=sizeof(struct sockaddr);
    _newFd=::accept(_sockFd,NULL,&len);//等待连接的阻塞点;
    cout<<"new fileDescription="<<_newFd<<endl;
}
void Socket::sendm(){
    int ret =send(_newFd,"helloworld",11,0);
    if(0==ret){
        cout<<"EOF"<<endl;
        return;
    }
    else if(-1==ret){
        cout<<">>error:send"<<endl;
        return;
    }else{
        cout<<"Server send:\"helloworld\""<<endl;
    }
    close(_newFd);
}
Socket::Socket(const string &ip,unsigned short port)
    :_ip(ip),_port(port){
        memset(&_servAddr,0,sizeof(_servAddr));
        _servAddr.sin_family=AF_INET;
        _servAddr.sin_addr.s_addr=inet_addr(_ip.c_str());
        _servAddr.sin_port=htons(_port);
    }
Socket::Socket(sockaddr_in & servAddr)
    :_servAddr(servAddr){}
void Socket::initial(){        
    _sockFd=socket(AF_INET,SOCK_STREAM,0);    
    }
int main(){
    Socket server("127.0.0.1",8888);
    server.initial();
    server.acceptor();
    server.sendm();
    return  0;
}
