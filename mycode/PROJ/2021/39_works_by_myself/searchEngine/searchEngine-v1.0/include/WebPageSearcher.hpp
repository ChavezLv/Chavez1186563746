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

#include "TcpConnection.hpp"
#include <iostream>
#include <string>
using std::string;

namespace wd{

class WebPageSearcher{
public:
    WebPageSearcher(const string &sought, const TcpConnectionPtr &conn)
    :_msg(sought)
    , _conn(conn){

    }
    void doQuery(){
        std::cout << "WebPageSearcher::doQuery()" <<std::endl;
        string response = _msg;//模拟回显服务
		_conn->sendInLoop(response);
    }

private:
    string _msg;
    TcpConnectionPtr _conn;

};

}//end of namespace wd

#endif //__WD_WEBPAGESEARCHER_HPP__
