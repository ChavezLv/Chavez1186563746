/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：testjson.cc
*   @author     ：anonymous
*   @date       ：2021年09月03日 09:44:28
*   @description：
*
================================================================*/
#include "../include/json/json.hpp"
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <algorithm>

using namespace nlohmann;
class JsonMsg{
public:
    JsonMsg(int msgLength = 0, int msgType = 0, string msgContent = "")
    : _msgLength(msgLength)
    , _msgType(msgType)
    , _msgContent(msgContent){

    }

    void toJson(json& js) {
        js = json{  {"msgLength", _msgLength}, {"msgType", _msgType}, {"msgContent", _msgContent} };
    }

    void fromJson(const json& js) {
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


string test01(){
    string line;
    cout << ">> pls input query word:";
    getline(cin, line);
    json js;
    JsonMsg(line.size(), 1, line).toJson(js);
    std::stringstream ss;
    ss << js;
    ss >> line;
    cout << "js to string : " << line << endl;

    JsonMsg msg;
    json string_to_json = line;// error, 
    msg.fromJson(js);
    /* cout << "string to js msg.getMsgLen: " << msg.getMsgLen() \ */
    /*     << "msg.getMsgType: " << msg.getMsgType() \ */
    /*     << "msg.getMsgContent: " << msg.getMsgContent() << endl; //error*/

    return line;

}
int main(int argc, char* argv[])
{

    string line = test01();

    JsonMsg msg;
    /* json string_to_json = line;//error */
    json string_to_json;
    std::stringstream ss;
    ss << line;
    ss >> string_to_json;
    cout << "string_to_json" << string_to_json << endl;
    msg.fromJson(string_to_json);
    cout << "string to js msg.getMsgLen: " << msg.getMsgLen() \
        << "msg.getMsgType: " << msg.getMsgType() \
        << "msg.getMsgContent: " << msg.getMsgContent() << endl;
    
    return 0;
}


