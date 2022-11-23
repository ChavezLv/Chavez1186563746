/**
 * Project netli_v1.0
 */


#include "TcpConnection.h"
#include <sstream>

namespace wd{

/**
 * TcpConnection implementation
 */



/**
 * @return string
 */
string TcpConnection::receive() {
    char buf[2<<15] = { 0 };
    _sockIO.readline(buf, sizeof(buf));
    return string(buf);

}

/**
 * @param const string & msg
 * @return void
 */
void TcpConnection::send(const string & msg) {
    _sockIO.writen(msg.c_str(), msg.size());
    return;
}

/**
 * @return string
 */
string TcpConnection::toString() const{
    std::ostringstream oss;
    oss << "tcp " 
        << _localAddr.ip() << ":" << _localAddr.port() << " --> "
        << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in localAddr;
    socklen_t len = sizeof(localAddr);
    ::memset(&localAddr, 0, sizeof(localAddr));
    if(0 != getsockname(_sock.fd(), (struct sockaddr*)&localAddr, &len)){
        perror("getsockname");
    } 

    return InetAddress(localAddr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in localAddr;
    socklen_t len = sizeof(localAddr);
    ::memset(&localAddr, 0, sizeof(localAddr));
    if(0 != getpeername(_sock.fd(), (struct sockaddr*)&localAddr, &len)){
        perror("getperrname");
    } 

    return InetAddress(localAddr);
    
}

}//end of namespace wd
