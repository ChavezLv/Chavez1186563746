#include"Acceptor.h"
Acceptor::Acceptor(const string &ip,unsigned short port)
        :_serverAddr(ip,port){}
Acceptor::~Acceptor(){}


void Acceptor::ready(){
    setReusedAddr();
    setReusedPort();
    bind();
    listen();
}

int Acceptor::fd()const{
    return _sockListen.fd();
}

int Acceptor::accept(){
    int connfd=::accept(_sockListen.fd(),nullptr,nullptr);
    if(-1==connfd){
        perror("listen");
        return -1;
    }
    return connfd;
}

void Acceptor::setReusedAddr(){
    int opt=1;
    int ret=setsockopt(_sockListen.fd(),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(ret){
        perror("setsockopt-address");
        return;
    }
}
void Acceptor::setReusedPort(){
    int opt=1;
    int ret =setsockopt(_sockListen.fd(),SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    if(ret==-1){
        perror("setsockopt-port");
        return;
    }
}
void Acceptor::bind(){
    int ret=::bind(\
                _sockListen.fd(),\
                (struct sockaddr*)_serverAddr.getInetAddPtr(),\
                sizeof(_serverAddr));
    if(-1==ret){
        perror("bind server to the network ");
        return;
    }
}
void Acceptor::listen(){
    int ret=::listen(_sockListen.fd(),128);
    if(-1==ret){
        perror("listen ");
        return;
    }
}
