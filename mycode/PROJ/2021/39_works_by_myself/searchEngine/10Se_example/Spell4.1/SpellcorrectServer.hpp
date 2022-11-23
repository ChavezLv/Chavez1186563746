#ifndef __SPELLCORRECTSERVER_H__
#define __SPELLCORRECTSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.hpp"
#include "Configuration.h"
#include "CacheManage.h"
namespace wd{
class Configuration;
class SpellcorrectServer
{
public:
    SpellcorrectServer(const string&);
    void start();
    void stop();
    void onConnection(const TcpConnectionPtr&);
    void onMessage(const TcpConnectionPtr&);
    void onClose(const TcpConnectionPtr&);
    ~SpellcorrectServer(){
        stop();
    }
private:
    yt::Configuration _mycfg;
    ThreadPool _threadPool;
    TcpServer _tcpServer;
    CacheManage _myCacheManger;
};

}//end of namespace;
#endif
