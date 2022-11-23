/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：SearchEngineServer.hpp
*   @author     ：anonymous
*   @date       ：2021年09月01日 22:01:39
*   @description：
*
================================================================*/

#include "TcpServer.hpp"
#include "Threadpool.hpp"

#ifndef __WD_SEARCHENGINESERVER_HPP__
#define __WD_SEARCHENGINESERVER_HPP__

namespace wd{


class SearchEngineServer{
public:
    SearchEngineServer(const string & ip, short port);// 构造函数
    void start();// 开始提供服务
    void stop();//end service
    void onConnection(const TcpConnectionPtr &conn);//被注册回调函数， 提供给TcpServer 使用
    void onMessage(const TcpConnectionPtr & conn);//被注册回调函数， 提供给TcpServer 使用
    void onClose(const TcpConnectionPtr & conn);// 被注册回调函数， 提供给TcpServer 使用
    void doTaskThread(const TcpConnectionPtr &conn, const string & msg); //该方法由线程池的某一个线程执行

private:

    TcpServer _tcpServer;// TCP 通信模块
    Threadpool _pool; //线程池
    KeyRecommander _keyRecommander; //关键字推荐模块
    WebPageSearcher _webSearcher;// 网页查询模块
    ProtocolParser _protocolParser; //协议解析模块
};

}//end of namespace wd

#endif //__WD_SEARCHENGINESERVER_HPP__
