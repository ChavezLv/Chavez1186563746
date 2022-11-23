/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：WebPageSearcher.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 14:56:23
*   @description：
*
================================================================*/


#ifndef __WD_WEBPAGESEARCHER_HPP__
#define __WD_WEBPAGESEARCHER_HPP__

/* #include "JsonMsg.hpp" */
#include "TcpConnection.hpp"
#include "WebPageQuery.hpp"

#include <iostream>
#include <string>
using std::string;

namespace wd{

class WebPageSearcher{
public:
    WebPageSearcher(const string &sought, const TcpConnectionPtr &conn)
    :_sought(sought)
    , _conn(conn){

    }
    
    void doQuery(){
        std::cout << "WebPageSearcher::doQuery()" <<std::endl;
        WebPageQuery webQuery;
        string res = webQuery.doQuery(_sought);//直接就是返回json格式的字符串了
std::cout << "after WebPageSearcher::doQuery()" <<std::endl;

        response(res);
    }

    void response(const string &res){
        /* string response = JsonMsg(_sought.size(), 200, _sought).toJsonLayoutStream();//模拟回显服务 */
		_conn->sendInLoop(res);
        
    }

private:
    string _sought;// void doQuery(const string &str){} 形式下这个没有用
    TcpConnectionPtr _conn;



};

}//end of namespace wd

#endif //__WD_WEBPAGESEARCHER_HPP__
