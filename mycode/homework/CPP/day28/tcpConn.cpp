#include"tcpConn.h"
#include"eventLoop.h"
TcpConnection::TcpConnection(int fd,EventLoop *loop)
    :_loop(loop)
    ,_sockIO(fd)
    ,_sock(fd)
    ,_localAddr(getLocalAddr())
    ,_peerAddr(getPeerAddr()){}

TcpConnection::~TcpConnection(){}


void TcpConnection::send(const string &msg){
    _sockIO.writen(msg.c_str(),msg.size());
}

void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
    }
}
string TcpConnection::receive(){
    char buff[65535]={0};
    _sockIO.readLine(buff,sizeof(buff));
    return  string (buff);
}

/*sign in callback function*/
void TcpConnection::setConnectionCallback(const TcpConnCallBack& cb){
    _onConn=cb;
}
void TcpConnection::setMessageCallback(const TcpConnCallBack &cb){
    _onMsg=cb;
}
void TcpConnection::setCloseCallback(const TcpConnCallBack &cb){
    _onClose=cb;
}


/*  using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;*/
void TcpConnection::handleConnectionCallback(){
    if(_onConn){
        _onConn(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback()
{
    if(_onMsg)
    {
        _onMsg(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}



/*assist function*/
InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in tmp;
    socklen_t len=sizeof(struct sockaddr);
    int ret=getsockname(_sock.fd(),(struct sockaddr*)&tmp,&len);
    if(-1==ret){
        perror("getsockname");
    }
    return InetAddress(tmp);
}
InetAddress TcpConnection::getPeerAddr(){
    
    struct sockaddr_in tmp;
    socklen_t len=sizeof(struct sockaddr);
    int ret=getpeername(_sock.fd(),(struct sockaddr*)&tmp,&len);
    if(-1==ret){
        perror("getsockname");
    }
    return InetAddress(tmp);
}
string TcpConnection::toString(){
    std::ostringstream oss;
    oss<<_localAddr.ip()<<":"
       <<_localAddr.port()<<"--->" 
       <<_peerAddr.ip()<<":"
       <<_peerAddr.port();
    return oss.str();
}
bool TcpConnection::isClosed(){
    char buff[1024];
    int ret =::recv(_sock.fd(),buff,10,MSG_PEEK);
    return 0==ret;
}
