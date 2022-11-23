/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：ProtocolParser.hpp
*   @author     ：anonymous
*   @date       ：2021年09月01日 22:17:25
*   @description：
*
================================================================*/


#ifndef __WD_PROTOCOLPARSER_HPP__
#define __WD_PROTOCOLPARSER_HPP__

#include "TcpConnection.hpp"
#include <string>

using std::string;

namespace wd{

class ProtocolParser{
public:
    ProtocolParser(const string & msg, const TcpConnectionPtr & conn)
    : _msg(msg)
    , _conn(conn){

    }

    void doParse(){
        if(_msg == string("1\n")){
            string msg = "key";
            /* cout << "Task::daParse() is running" << msg << endl; */
            _conn->sendInLoopParse(std::pair<int, std::pair<int, std::string> >{_conn->fd(), {1, msg}});
        }else{
            string msg = "web";
            _conn->fd();
            _conn->sendInLoopParse(std::pair<int, std::pair<int, std::string> >{_conn->fd(), {2, msg}});
        }
    }
private:
    string _msg;
	TcpConnectionPtr _conn;

};


}//end of namespace wd

#endif //__WD_PROTOCOLPARSER_HPP__
