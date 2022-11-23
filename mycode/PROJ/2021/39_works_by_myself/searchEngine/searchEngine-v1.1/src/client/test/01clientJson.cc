 ///
 /// @file    clientTest.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-02-03 21:10:10
 ///
#include "../include/json/json.hpp"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <sstream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

using namespace nlohmann;
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


void test0() 
{
    int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0) {
        perror("socket");
        return ;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.10.132");
    /* serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); */
    socklen_t length = sizeof(serverAddr);

    if(::connect(clientfd,(struct sockaddr*)&serverAddr, length) < 0) {
        perror("connect");
        close(clientfd);
        return;
    }
    printf("conn has connected!...\n");

    while(1) {
        string line;
        cout << ">> pls input query word:";
        getline(cin, line);
        //--------------
        string jsonLayout = JsonMsg(line.size(), 1, line).toJsonLayoutStream(); /* cout << js << endl; //ok*/
        

        /* JsonMsg msg; */
        /* msg.fromJson(js); */
/* cout << msg.getMsgType() << msg.getMsgContent() << endl; */
        /* line.append("\n"); */

        /* std::stringstream ss; */
        /* ss << js; */
        /* ss >> line; */
        /* string sendJson = js; */
        /* line << js; */
        /* cout << "js to string : " << line << endl; */

        /* ss << line; */
        /* ss >> js; */
        /* msg.fromJson(js); //error */

        /* json string_to_json = line;//error */
        /* msg.fromJson(string_to_json); //error */
/* cout << "string to js" << msg.getMsgLen() << msg.getMsgType() << msg.getMsgContent() << endl; */
        
        //1. 客户端先发数据
        jsonLayout.append("\n");
        send(clientfd, jsonLayout.data(), jsonLayout.size(), 0);
        /* cout << "sendJson " <<  sendJson << endl; */ 
        /* send(clientfd, sendJson.data(), sendJson.size(), 0); */
        /* close(clientfd); */

        char buff[1024] = {0};
        recv(clientfd, buff, sizeof(buff), 0);

        JsonMsg msg;
        msg.fromJsonLayoutStream(buff);
cout << "recv msg from server:" << msg.getMsgLen() << " " \
    << msg.getMsgType() << " " << msg.getMsgContent() << endl;
        
        /* printf("recv msg from server: %s\n", buff); */

        /* sleep(1); */
        /* close(clientfd); */
    }

    close(clientfd);
} 

int main(void)
{
    test0();
    return 0;
}
