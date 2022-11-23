#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "Socket.hpp"
#include "SocketIO.hpp"

#include <string>
using std::string;

namespace spellCorrect
{

class Client
{
public:
    Client();

    void connect(const string& ip,unsigned short port);
    void doServer();

private:
    Socket _sock;
    SocketIO _socketIO;
};


}//end of namepace spellCorrect

#endif
