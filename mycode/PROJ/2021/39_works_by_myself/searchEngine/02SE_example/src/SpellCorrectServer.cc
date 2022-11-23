#include "../include/MyLog.hpp"
#include "../include/SpellCorrectServer.hpp"
#include "../include/MyTask.hpp"
#include "../include/Myconf.hpp"
#include <stdio.h>
#include <string>
#include <json/json.h>

using std::stoi;
using std::string;
namespace wd
{

SpellCorrectServer::SpellCorrectServer(Myconf &conf)
:_conf(conf)
,_tcpServer(conf.getConfigMap()["serverIP"],stoi(conf.getConfigMap()["port"]))
,_threadpool(stoi(conf.getConfigMap()["threadpoolCap"]),
             stoi(conf.getConfigMap()["taskQueSize"]))
{
}

void SpellCorrectServer::onConnection(const TcpConnectionPtr &conn)
{
    /* printf("%s has connectd!\n",conn->toString().c_str()); */
    LogInfo("%s has connectd!\n",conn->toString().c_str());
    conn->send("welcome to server.");
}

void SpellCorrectServer::onMessage(const TcpConnectionPtr &conn)
{
    int32_t len = conn->recvInt32();
    /* std::string msg = conn->receive(); */
    
    /* printf("len=%d\n",len); */
    char buff[1024]={0};
    conn->receiveN(buff,len);
    //json解码
    Json::Value root;
    Json::Reader reader;

    if(!reader.parse(buff,root)){
        /* printf("Json parse fail!\n"); */
        LogError("Json parse fail!\n");
        return;
    }
    
    /* printf("buf=%s\n",buff); */
    int i=0;
    string msg = root["queryWord"][i].asString();
    /* printf("msg=%s\n",msg.c_str()); */
    MyTask task(msg,conn); 
    _threadpool.addTask(std::bind(&MyTask::response,&task));
    /* printf("exit onMessage\n"); */
}

void SpellCorrectServer::onClose(const TcpConnectionPtr &conn)
{
    /* printf("%s has closed!\n",conn->toString().c_str()); */
    LogInfo("%s has closed!\n",conn->toString().c_str());
}

void SpellCorrectServer::start()
{

    _threadpool.start();
 
    using namespace std::placeholders;

    _tcpServer.setConnectionCallback(std::bind(&SpellCorrectServer::onConnection,this,_1));
    _tcpServer.setMessageCallback(std::bind(&SpellCorrectServer::onMessage,this,_1));
    _tcpServer.setCloseCallback(std::bind(&SpellCorrectServer::onClose,this,_1));
     
    _tcpServer.start();

}

}//end of namespace wd
