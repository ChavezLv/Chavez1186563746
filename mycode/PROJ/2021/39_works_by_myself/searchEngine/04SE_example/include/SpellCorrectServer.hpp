#ifndef __SPELLCORRECTSERVER_HPP__
#define __SPELLCORRECTSERVER_HPP__

#include "Configuration.hpp"
#include "TcpServer.hpp"
#include "ThreadPool.hpp"

namespace spellCorrect
{

class SpellCorrectServer
{
public:
    SpellCorrectServer(const string& cfgName);

    void start();
    
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn);
    void onClose(const TcpConnectionPtr& conn);
private:
    Configuration& _conf;
    TcpServer _tcpServer;
    ThreadPool _threadPool;
};
}//end of namespace spellCorrect

#endif
