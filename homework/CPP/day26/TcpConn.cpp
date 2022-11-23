#include"TcpConn.h"
TcpConn::TcpConn(int fd)
     :_sock(fd)
     ,_sockIO(fd)
     ,_localAddr(getPeerAddr())
     ,_peerAddr(getPeerAddr()){}
TcpConn::~TcpConn(){}
void TcpConn::send(const string &msg){
    _sockIO.writen(msg.c_str(),msg.size());
}
string TcpConn::receive(){
    char buff[65535]={0};
    _sockIO.readLine(buff,sizeof(buff));
    return string(buff);
}
string TcpConn::toString(){
    ostringstream oss;
    oss<<_localAddr.ip()":"<<_localAddr.port()<<"--->"<<_peerAddr.ip()<<":"<<_peerAddr.port();
    return oss.str();
}
InetAddress TcpConn::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len =sizeof(struct sockaddr);
    int ret =getsockname(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(-1==ret){
        perror("getsockname");
    }
    return  InetAddress(addr);
}
InetAddress TcpConn::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(struct sockaddr);
    int ret=getpeername(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(-1==ret){
        perror("getsockname");
    }
    return  InetAddress(addr);
    
}
