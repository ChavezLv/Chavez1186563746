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
#include "JsonMsg.hpp"
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
        JsonMsg msg;
        msg.fromJsonLayoutStream(_msg);
        
// std::cout << "doParse()" << _msg << std::endl;
        // if(msg.getMsgType() == 1){
            _conn->sendInLoopParse(std::pair<int, std::pair<int, std::string> >{_conn->fd(), \
                                   {msg.getMsgType(), msg.getMsgContent()}});
        // }else if(msg.getMsgType() == 2){
        //     _conn->sendInLoopParse(std::pair<int, std::pair<int, std::string> >{_conn->fd(), 
        //                            {msg.getMsgType(), msg.getMsgContent()}});
        // }
    }
private:
    string _msg;
	TcpConnectionPtr _conn;

};


}//end of namespace wd

#endif //__WD_PROTOCOLPARSER_HPP__
