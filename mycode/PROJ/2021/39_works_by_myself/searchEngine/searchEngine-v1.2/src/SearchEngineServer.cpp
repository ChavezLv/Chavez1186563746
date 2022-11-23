/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：SearchEngineServer.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 14:53:48
*   @description：
*
================================================================*/


#include "SearchEngineServer.hpp"


namespace wd{

SearchEngineServer::SearchEngineServer(const string &configurationFilePath)
:_isStart(false)
,_configurationPtr(Singleton<Configuration>::getInstance(configurationFilePath)) ,_threadpool(stoi(_configurationPtr->getConfigMap()[THREAD_NUM_KEY]), stoi(_configurationPtr->getConfigMap()[QUEUE_CAPACITY_KEY]) )
, _tcpServer(stoi(_configurationPtr->getConfigMap()[PORT_KEY]), _configurationPtr->getConfigMap()[IP_KEY])
/* , _keyRecommander() */
/* , _webSearcher() */{

}// 构造函数

void SearchEngineServer::start(){
    _threadpool.start();
    _tcpServer.setAllCallbacks(
         std::bind(&SearchEngineServer::onConnection, this, std::placeholders::_1),
         std::bind(&SearchEngineServer::onMessage, this, std::placeholders::_1),
         std::bind(&SearchEngineServer::onClose, this, std::placeholders::_1),
         std::bind(&SearchEngineServer::onParse, this, std::placeholders::_1, std::placeholders::_2) );

    _tcpServer.start();
    _isStart = true;

}// 开始提供服务

void SearchEngineServer::stop(){
    if(_isStart){
        _tcpServer.stop();
        _threadpool.stop();
        _isStart = false;
    }
}//end service

void SearchEngineServer::onConnection(const TcpConnectionPtr &conn){
    cout << ">> tcp " << conn->toString() << " has connected!" << endl;
}//被注册回调函数， 提供给TcpServer 使用

void SearchEngineServer::onMessage(const TcpConnectionPtr & conn){
    string msg = conn->receive();
// cout << "SearchEngineServer::onMessage(const TcpConnectionPtr & conn)" << msg << endl;
    doTaskThread(conn, msg);
};//被注册回调函数， 提供给TcpServer 使用

void SearchEngineServer::onParse(const TcpConnectionPtr &conn, const std::pair<int, std::string>& msgPacket){
    string msg = msgPacket.second;
    if(msgPacket.first == 1){
        KeyRecommender keyRecommender(msg, conn, _configurationPtr);   
        _threadpool.addTask(std::bind(&KeyRecommender::doProcess, keyRecommender));
    }else if(msgPacket.first == 2){
        WebPageSearcher webSearcher(msg, conn);
        _threadpool.addTask(std::bind(&WebPageSearcher::doQuery, webSearcher));
    }
}

void SearchEngineServer::onClose(const TcpConnectionPtr & conn){
    cout << ">> tcp " << conn->toString() << " has closed!" << endl;

}// 被注册回调函数， 提供给TcpServer 使用

void SearchEngineServer::doTaskThread(const TcpConnectionPtr &conn, const string & msg){
    //加入协议解析的代码
    ProtocolParser protocolParser(msg, conn);
    _threadpool.addTask(std::bind(&ProtocolParser::doParse, protocolParser));
    
} //该方法由线程池的某一个线程执行


}//end of namespace wd

