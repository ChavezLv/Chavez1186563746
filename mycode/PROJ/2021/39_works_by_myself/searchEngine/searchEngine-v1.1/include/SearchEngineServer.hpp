/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：SearchEngineServer.hpp
*   @author     ：anonymous
*   @date       ：2021年09月01日 22:01:39
*   @description：
*
================================================================*/

#include "Threadpool.hpp"
#include "TcpServer.hpp"

#include "Configuration.hpp"
#include "ProtocolParser.hpp"
#include "KeyRecommender.hpp"
#include "WebPageSearcher.hpp"

#ifndef __WD_SEARCHENGINESERVER_HPP__
#define __WD_SEARCHENGINESERVER_HPP__

namespace wd{

#define THREAD_NUM_KEY "THREAD_NUM" 
#define QUEUE_CAPACITY_KEY "QUEUE_CAPACITY" 
#define IP_KEY "IP"
#define PORT_KEY "PORT"
    
class SearchEngineServer{
public:
    SearchEngineServer(const string &configurationFilePath);// 构造函数
    void start();// 开始提供服务
    void stop();//end service
    void onConnection(const TcpConnectionPtr &conn);//被注册回调函数， 提供给TcpServer 使用
    void onMessage(const TcpConnectionPtr & conn);//被注册回调函数， 提供给TcpServer 使用
    void onClose(const TcpConnectionPtr & conn);// 被注册回调函数， 提供给TcpServer 使用
    void onParse(const TcpConnectionPtr & conn, const std::pair<int, std::string>& msgtype);

    void doTaskThread(const TcpConnectionPtr &conn, const string & msg); //该方法由线程池的某一个线程执行

private:
    bool _isStart;
    ConfigurationPtr _configurationPtr; 
    Threadpool _threadpool; //线程池
    TcpServer _tcpServer;// TCP 通信模块
    /* KeyRecommander _keyRecommander; //关键字推荐模块 */
    /* WebPageSearcher _webSearcher;// 网页查询模块 */
    /* ProtocolParser _protocolParser; //协议解析模块 , 只用到一个函数即可*/
};

}//end of namespace wd

#endif //__WD_SEARCHENGINESERVER_HPP__
