#include "SpellcorrectServer.hpp"
#include "TcpConnection.h"
#include "MyTask.h"
#include "Mydict.h"
#include "CacheManage.h"

using std::bind;
using std::stoi;
using std::placeholders::_1;

namespace  wd{

/* ThreadPool* pool; */
SpellcorrectServer::SpellcorrectServer(const string&configPath)
:_mycfg(static_cast<yt::Configuration>(configPath))
,_threadPool(static_cast<size_t>(stoi(_mycfg.getMap()["ThreadpoolCap"])),static_cast<size_t>(stoi(_mycfg.getMap()["TaskQueSize"])))
,_tcpServer(_mycfg.getMap()["SeverIP"],static_cast<unsigned int>(stoi(_mycfg.getMap()["Port"])))
,_myCacheManger(_mycfg)
{
    Mydict::getInstance()->initIndexTable(_mycfg.getMap()["indexpath"],_mycfg.getMap()["dictionary"]);
    /* cout<<"dict has build !"<<endl; */
    /* CacheManage::getInstance()->setConf(_mycfg); */
    
    /* _mycfg=Configuration(configPath)); */
    /* string ip=_mycfg.getMap()["ip"]; */
    /* unsigned int port=stoi(_mycfg.getMap()["port"]); */
    /* _tcpServer=std::move(TcpServer(ip,port)); */
    /* pool=&_threadPool; */
}

void SpellcorrectServer::onConnection(const TcpConnectionPtr& conn){
    cout<<conn->toString()<<"has connect!"<<endl;
    conn->send("welcome to server!");
}

void SpellcorrectServer::onMessage(const TcpConnectionPtr& conn){
   string msg=conn->receive();
   /* cout<<"In onMessage search word is "<<msg<<endl; */
   msg.pop_back();
   /* cout<<">In onMessage word size = "<<msg.size()<<endl; */
   MyTask mytask(conn,msg,stoi(_mycfg.getMap()["ResultSize"]),_myCacheManger);
   cout<<"task has establish!"<<endl;
   _threadPool.addTask(bind(&MyTask::execute,mytask));
}

void SpellcorrectServer::onClose(const TcpConnectionPtr& conn){
    cout<<conn->toString()<<"has closed!"<<endl;
}

void SpellcorrectServer::start(){
    _threadPool.start();
    cout<<"_threadPool has start !"<<endl;    
    _tcpServer.setNewConnectionCallBack(bind(&SpellcorrectServer::onConnection,this, _1));
    _tcpServer.setMessageCallBack(bind(&SpellcorrectServer::onMessage,this, _1));
    _tcpServer.setCloseCallBack(bind(&SpellcorrectServer::onClose,this, _1));

    cout<<"_tcpServer has start !"<<endl;
    _tcpServer.start();
    
}

void SpellcorrectServer::stop(){
    _threadPool.stop();
    /* _tcpServer. */
}
}//end of namespace;
