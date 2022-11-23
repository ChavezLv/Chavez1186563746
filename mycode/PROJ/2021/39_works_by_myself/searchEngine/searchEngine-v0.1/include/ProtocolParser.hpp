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

#include <string>
using std::string;

namespace wd{

class ProtocolParser{
public:
    ProtocolParser(const string & msg)
    : _msg(msg){

    }
    void doParse(){

    }
private:
    string _msg;

};


}//end of namespace wd

#endif //__WD_PROTOCOLPARSER_HPP__
