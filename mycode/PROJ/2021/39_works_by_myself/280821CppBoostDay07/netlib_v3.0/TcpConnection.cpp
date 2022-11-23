/*================================================================
*   
*   @file       ：TcpConnection.cpp
*   @date       ：2021年08月29日 22:03:33
*   @description：
*
================================================================*/


#include "TcpConnection.hpp"
#include <sstream>
using std::ostringstream;

namespace wd{

string TcpConnection::receive(){
    char buf[1 << 16] = { 0 };//65536
    _socketIO.readline(buf, sizeof(buf));//readline()以"\n" 区分数据流的每一行
    return string(buf);
}

void TcpConnection::send(const string &msg){
    _socketIO.writen(msg.c_str(), msg.size());
}

InetAddress TcpConnection::getlocalAddr(){
    struct sockaddr_in localAddr;
    socklen_t len = sizeof(localAddr);
    ::memset(&localAddr, 0, sizeof(localAddr));
    if(0 != getsockname(_socket.fd(), (struct sockaddr *)&localAddr,\
                        &len)){
        perror("getsockname");
    }

    return InetAddress(localAddr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in peerAddr;
    socklen_t len = sizeof(peerAddr);
    ::memset(&peerAddr, 0, sizeof(peerAddr));
    if(0 != getpeername(_socket.fd(), (struct sockaddr *)&peerAddr,\
                        &len)){
        perror("getpeername");
    }

    return InetAddress(peerAddr);
}

string TcpConnection::toString() const{
    std::ostringstream oss;
    oss << "tcp"
        << _localAddr.ip() << ":" << _localAddr.port() << "-->"
        << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}

void TcpConnection::handleConnectionCallback()
{
	//之前调用回调函数的时候就是传智能指针tcp对象本身
	//就是类内部获取this的指针, 就要用shared_from_this()
	if(_onConnection) {
		_onConnection(shared_from_this());
	}
}

void TcpConnection::handleMessageCallback()
{
	if(_onMessage) {
		_onMessage(shared_from_this());
	}
}

void TcpConnection::handleCloseCallback()
{
	if(_onClose) {
		_onClose(shared_from_this());
	}
}

bool TcpConnection::isClosed() const
{
	int nret = -1;//recv不要移走数据, 只是查看一下就行!
	char buff[64] = {0};
	do {
		nret = recv(_socket.fd(), buff, sizeof(buff), MSG_PEEK);
	}while(nret == -1 && errno == EINTR);
	return 0 == nret;
}


}//end of namespace wd

