/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：JsonMsg.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 22:50:56
*   @description：
*
================================================================*/


#ifndef __WD_JSONMSG_HPP__
#define __WD_JSONMSG_HPP__

#include "json/json.hpp"
#include "iostream"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

namespace wd{

using json = nlohmann::json;

class JsonMsg{
public:
    JsonMsg(int msgLength = 0, int msgType = 0, string msgContent = "")
    : _msgLength(msgLength)
    , _msgType(msgType)
    , _msgContent(msgContent){

    }

    
    void toJson(json& js) {
        string line;
        js = json{  {"msgLength", _msgLength}, {"msgType", _msgType}, {"msgContent", _msgContent} };
    }

    //直接构造对象, 得到json格式字符流
    string toJsonLayoutStream(){
        string jsonLayout;
        json js;
        js = json{  {"msgLength", _msgLength}, {"msgType", _msgType}, {"msgContent", _msgContent} };
        std::stringstream ss;
        ss << js;
        ss >> jsonLayout;
        return jsonLayout;
    }

    string toJsonLayoutStreamForWebSearch(const json &js){
        /* std::cout << "JsonMsg:: toJsonLayoutStreamForWebSearch" << js <<  std::endl; */
        string jsonLayout;
        std::stringstream ss;
        ss << js;

        string word;
        while(ss >> word){
            jsonLayout.append(word).append(" ");
        }

        // js["msgLength"] = jsonLayout.size();//重新确认一下大小
        // ss.clear();
        // ss << js;
        // ss >> jsonLayout;
        return jsonLayout;
        
    }

    void fromJson(const json& js) {
        js.at("msgLength").get_to(_msgLength);
        js.at("msgType").get_to(_msgType);
        js.at("msgContent").get_to(_msgContent);
    }
    void fromJsonLayoutStream(const string& jsonLayout){
        std::stringstream ss;
        json js;
        ss << jsonLayout;
        ss >> js;
        js.at("msgLength").get_to(_msgLength);
        js.at("msgType").get_to(_msgType);
        js.at("msgContent").get_to(_msgContent);

    }

    int getMsgLen(){ return _msgLength; }
    int getMsgType(){ return _msgType; }
    string getMsgContent(){ return _msgContent; }

private:
    int _msgLength;
    int _msgType;
    string _msgContent;
};


}//end of namespace wd

#endif //__WD_JSONMSG_HPP__
