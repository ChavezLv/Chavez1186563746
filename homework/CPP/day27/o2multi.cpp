#include<iostream>
#include<sys/select.h>
#include"o2multi.h"
using std::cout;
using std::endl;
/* 1,socket
 * 2,bind
 * 3,listen
 * 4,accept
 * 5,send/recv*/
void Socket::loop(){
    fd_set oldset;
    FD_ZERO(&oldset);
    FD_SET(_sockFd,&oldset);
    FD_SET(STDIN_FILENO,&oldset);
    int maxfd=_sockFd,countFd=0,clientFd[128]={0};
    while(1){
        fd_set retset=oldset;
        select(maxfd+1,&retset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&retset)){
            char buff[1024]={0};
            int ret=read(STDIN_FILENO,buff,1024);
            if(ret>0){
                for(int i=0;i<countFd;++i){
                    if(clientFd[i]!=0)
                    {
                        send(clientFd[i],buff,ret+1,0);
                    }
                }
            }
        }
        else if(FD_ISSET(_sockFd,&retset)){
            socklen_t len=sizeof(struct sockaddr);
            int newfd=::accept(_sockFd,NULL,&len);//等待连接的阻塞点;
            cout<<"the client has link which filedescription="<<newfd<<endl;
            FD_SET(newfd,&oldset);
            for(int i=0;i<128;++i){
                if(clientFd[i]==0){
                    clientFd[i]=newfd;
                    countFd++;
                    cout<< "clientFd["<<i<<"]="<<clientFd[i]<<endl;
                    cout <<"countFd="<<countFd<<endl;
                    break;
                }
            }
            maxfd=newfd;
        }else {
            for(int i=0;i<countFd;++i){
                if(FD_ISSET(clientFd[i],&retset)){
                    char buff[1024]={0};
                    recv(clientFd[i],buff,1024,0);
                    cout<<"Server recv from"<<clientFd[i]<<": "<<buff<<endl;
                }
            }
        }
    }
}

Socket::~Socket(){
    close(_sockFd);
}
void Socket::acceptor(){
    bind(_sockFd,(struct sockaddr*)&_servAddr,sizeof(struct sockaddr_in));
    listen(_sockFd,1);//one to one;
    /* cout<<"new fileDescription="<<_newFd<<endl; */
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
    server.loop(); 
    return  0;
}
