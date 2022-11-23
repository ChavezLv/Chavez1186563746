#include "../../include/SpellCorrectServer.hpp"
#include "../../include/MyTask.hpp"

#include <iostream>
#include <functional>

using std::cout;
using std::endl;

namespace spellCorrect
{
SpellCorrectServer::SpellCorrectServer(const string& cfgName)
    :_conf(*Configuration::getInstance(cfgName))
     ,_tcpServer((_conf.getConfigMap())["ip"],stoi((_conf.getConfigMap())["port"]))
     ,_threadPool(4,10)
    {}

void SpellCorrectServer::start()
{
    _threadPool.start();

    _tcpServer.setConnectionCallback(std::bind(&SpellCorrectServer::onConnection,this,std::placeholders::_1));
    _tcpServer.setMessageCallback(std::bind(&SpellCorrectServer::onMessage,this,std::placeholders::_1));
    _tcpServer.setCloseCallback(std::bind(&SpellCorrectServer::onClose,this,std::placeholders::_1));
    _tcpServer.start();
}

void SpellCorrectServer::onConnection(const TcpConnectionPtr& conn)
{
    cout << conn->toString() << " has connected." << endl;
}

void SpellCorrectServer::onMessage(const TcpConnectionPtr& conn)
{
    string msg = conn->receive();
    cout << conn->toString() << " get msg: " << msg << endl;
    //交给任务队列
    MyTask task(msg,conn);
    _threadPool.addTask(std::bind(&MyTask::execute,task));
}

void SpellCorrectServer::onClose(const TcpConnectionPtr& conn)
{
    cout << conn->toString() << " has closed." << endl;
}

}//end of namespace spellCorrect

