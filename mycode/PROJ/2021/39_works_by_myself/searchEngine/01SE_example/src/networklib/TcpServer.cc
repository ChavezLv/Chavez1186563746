#include "../../include/networklib/TcpServer.hh"

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

TcpServer::TcpServer(const std::string & ip, unsigned short port)
    : _acceptor(ip, port)
    , _loop(_acceptor)
{}

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{ _loop.unloop(); }

void TcpServer::setConnectionCallback(Callback && cb)
{ _loop.setNewConnectionCallback(std::move(cb)); }

void TcpServer::setMessageCallback(Callback && cb)
{ _loop.setMessageCallback(std::move(cb)); }

void TcpServer::setCloseCallback(Callback && cb)
{ _loop.setCloseCallback(std::move(cb)); }

} // end of namespace wd
