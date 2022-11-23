#include<sys/socket.h>
#include<memory.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<string>
#include<iostream>
using std::cout;
using std::string;
using std::endl;
class Client{
public:
    Client(const string &ip,unsigned short port);
    void initial();
    void recieve();
private:
    struct sockaddr_in _cliAddr;
    string _ip;
    unsigned short _port;
    int _clientFd;
};
Client::Client(const string &ip,unsigned short port)
    :_ip(ip),_port(port){
    memset(&_cliAddr,0,sizeof(_cliAddr));
    _cliAddr.sin_family=AF_INET;
    _cliAddr.sin_addr.s_addr=inet_addr(_ip.c_str());
    _cliAddr.sin_port=htons(_port);
}
void Client::initial(){
    _clientFd=socket(AF_INET,SOCK_STREAM,0);
    cout<<"client connect"<<endl;

    int ret=connect(_clientFd,(struct sockaddr*)&_cliAddr,sizeof(struct sockaddr_in));
    cout<<"client connect"<<endl;
}
void Client::recieve(){
    char buff[1024];
    fd_set oldset;
    FD_SET(_clientFd,&oldset);
    FD_SET(STDIN_FILENO,&oldset);
    while(1){
        fd_set retset=oldset;
        select(_clientFd+1,&retset,NULL,NULL,NULL);
        if(FD_ISSET(_clientFd,&retset)){
            cout<<"data coming"<<endl;
            int ret =recv(_clientFd,buff,1024,0);
            if(0==ret){
                cout<<"EOF"<<endl;
                return;
            }
            else if(-1==ret){
                cout<<">>error:send"<<endl;
                return;
            }else{
                cout<<"Client recv: "<<buff<<endl;
            }
        }
        else if(FD_ISSET(STDIN_FILENO,&retset)){
            char buff[1024];
            cout<<"cin operator"<<endl;
            int ret=read (STDIN_FILENO,buff,1024);
            send(_clientFd,buff,ret,0);
        }
    }
    close(_clientFd);
}

int main(){
    Client ct("127.0.0.1",8888);
    ct.initial();
    ct.recieve();
    return 0;
}
