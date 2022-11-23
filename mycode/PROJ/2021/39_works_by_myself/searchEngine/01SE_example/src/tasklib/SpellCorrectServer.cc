#include "../../libs/Mylog.hh"
#include "../../include/tasklib/MyTask.hh"
#include "../../include/tasklib/Mydict.hh"
#include "../../include/tasklib/SpellCorrectServer.hh"
#include "../../include/cache/CacheManager.hh"
#include "../../include/cache/TimerThread.hh"

#include <iostream>
#include <memory>
#include <string>
using std::cout;
using std::endl;
using std::string;

namespace wd
{

SpellCorrectServer::SpellCorrectServer(const std::string & cfgFileName)
    : _conf(std::move(Configuration(cfgFileName)))
    , _tcpServer(_conf.getConfigMap()["ServerIP"],
        atoi(_conf.getConfigMap()["Port"].c_str()))
    , _threadpool(atoll(_conf.getConfigMap()["ThreadpoolCap"].c_str()),
        atoll(_conf.getConfigMap()["TaskQueSize"].c_str()))
    , _tThread(std::bind(&CacheManager::periodicUpdateCaches, CacheManager::getInstance()),
        atoll(_conf.getConfigMap()["UpdateCachesTimeInit"].c_str()),
        atoll(_conf.getConfigMap()["UpdateCachesTimePeri"].c_str()))
{
    // 初始化词典
    Mydict::createInstance();
    // 初始化CacheManager
    CacheManager::getInstance()->initCache(atoll(_conf.getConfigMap()["ThreadpoolCap"].c_str()),
        _conf.getConfigMap()["cachePath"].c_str());
}


void SpellCorrectServer::start()
{
    _threadpool.start();
    _tThread.start();

    using std::placeholders::_1;
    _tcpServer.setConnectionCallback(std::bind(&SpellCorrectServer::onConnection, this, _1));
    _tcpServer.setMessageCallback(std::bind(&SpellCorrectServer::onMessage, this, _1));
    _tcpServer.setCloseCallback(std::bind(&SpellCorrectServer::onClose, this, _1));

    _tcpServer.start();
}


void SpellCorrectServer::onConnection(TcpConnectionPtr conn)
{
    logInfo(conn->toString(), " : connected.");
}

void SpellCorrectServer::onMessage(TcpConnectionPtr conn)
{
    string msg = conn->receive();
    logInfo(conn->toString(), " : get -> ", msg);

    MyTask task(msg, conn);
    _threadpool.addTask(std::bind(&MyTask::execute, task));
}

void SpellCorrectServer::onClose(TcpConnectionPtr conn)
{
    logInfo(conn->toString(), " : closed.");
}


} // end of namespace wd
