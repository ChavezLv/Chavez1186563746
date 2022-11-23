/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：KeyRecommander.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 14:55:48
*   @description：
*
================================================================*/


#ifndef __WD_KEYRECOMMANDER_HPP__
#define __WD_KEYRECOMMANDER_HPP__

#include "TcpConnection.hpp"
#include <iostream>
#include <string>
using std::string;

namespace wd{

class KeyRecommander{
public:
    KeyRecommander(const string &word, const TcpConnectionPtr &conn)
    : _msg(word)
    , _conn(conn){

    }

    void doQuery(){
        std::cout << "KeyRecommande doQuery()" << std::endl;
        string response = _msg;//模拟回显服务
        _conn->sendInLoop(response);
    }
private:
    string _msg;
    TcpConnectionPtr _conn;

};

}//end of namespace wd

#endif //__WD_KEYRECOMMANDER_HPP__
