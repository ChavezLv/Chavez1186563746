#ifndef __SPELLCORRECTSERVER_HPP__
#define __SPELLCORRECTSERVER_HPP__

#include "TcpServer.hpp"
#include "Threadpool.hpp"
#include <memory>
#include <string>

namespace wd
{
class Myconf;
class TcpConnection;

/* using TcpConnectionPtr = std::shared_ptr<const TcpConnection&>; */

class SpellCorrectServer
{
public:
    /* SpellCorrectServer(const std::string &confFileName); */
    SpellCorrectServer(Myconf &conf);
    ~SpellCorrectServer() {}
    
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn);
    void onClose(const TcpConnectionPtr &conn);
    void start();
private:
    Myconf &_conf;
    TcpServer _tcpServer;
    Threadpool _threadpool;
};

}//end of namespace wd

#endif
