/* <SpellCorrectServer> -*- C++ -*- */
#ifndef __SPELLCORRECTSERVER_H__
#define __SPELLCORRECTSERVER_H__

#include "../networklib/TcpConnection.hh"
#include "../networklib/TcpServer.hh"
#include "../networklib/Threadpool.hh"
#include "../cache/TimerThread.hh"
#include "../locallib/Configuration.hh"

#include <memory>
#include <string>

namespace wd
{

class Configuration;

class SpellCorrectServer
    /* : public std::enable_shared_from_this<SpellCorrectServer> */
{
public:
    SpellCorrectServer(const std::string & cfgFileName);
    
    void start();

    // 提供给TcpServer使用的
    void onConnection(TcpConnectionPtr conn);
    void onMessage(TcpConnectionPtr conn);
    void onClose(TcpConnectionPtr conn);

private:
    Configuration _conf;
    TcpServer _tcpServer;
    Threadpool _threadpool;
    TimerThread _tThread;

};

}
#endif

