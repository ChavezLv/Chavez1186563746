/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Client.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 20:03:32
*   @description：
*
================================================================*/


#ifndef __WD_CLIENT_HPP__
#define __WD_CLIENT_HPP__

#include "Socket.hpp"
#include "SocketIO.hpp"
#include <string>
using std::string;

namespace wd{

class Client
{
public:
    Client();

    void connect(const string& ip,unsigned short port);
    void doServer();
    int fd() { return _sock.fd(); }

private:
    Socket _sock;
    SocketIO _socketIO;
};

}//end of namespace wd

#endif //__WD_CLIENT_HPP__
