#include "../include/JsonMsg.hpp"
using json = nlohmann::json;
#include <iostream>
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

template<typename _Tp>
class Solution {
public:
   
};
/* using namespace wd; */
string jsonLayOutNullResultString(){
    string title = "404 Not Found";
    /* string summary  = "Please try using a different keyword !"; */
    /* string url = "www.wangdao.com"; */
    string summary  = "Please try using a different key word!";
    string url = "www.wangdao.com";
    /* json js = */
    /* { */
    /*     {"msgLength", 1}, */
    /*     {"msgType", 200}, */
    /*     {"msgContent", {}} */
    /* }; */
    json js = 
    {
        {"length", 1},
        {"type", 200},
        {"webarray", {}
            /* { */
            /*             {//"1",// 没有用 */
            /*                 {"title", "b"}, */
            /*                 {"summary", "bcd"}, */
            /*                 {"url", "www.b.com"} */
            /*             }, */
            /*             { */
            /*                 {"title", "c"}, */
            /*                 {"summary", "cde"}, */
            /*                 {"url", "www.c.com"}, */
            /*             }, */
            /*             { */
            /*                 {"title", "d"}, */
            /*                 {"summary", "def"}, */
            /*                 {"url", "www.d.com"}, */
            /*             } */
            /*         } */ 
        }
    };

    js.at("/webarray"_json_pointer) = "msgContent";
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    js["msgContent"].push_back(
                               {
                                    {"title", "这几种主食对人体的影响不容忽视"},
                                    {"summary", "这几种主食对人体的影响不容忽视"},
                                    {"url", "http://health.people.com.cn/n1/2021/0517/c14739-32105346.html"},
                               }
                              );
    cout << "before to string " << endl;
    cout << js << endl;
    cout << "to string " << endl;
        string jsonLayout;
        std::stringstream ss;
        ss << js;
        string word;
        while(ss >> word){
            jsonLayout.append(word).append(string(" "));
        }
        cout << jsonLayout << endl;

    /* string response = JsonMsg().toJsonLayoutStreamForWebSearch(js); */

    cout << "b to str WebPageQuery::jsonlayoutNULLResultstring" << endl << js << endl;
    /* string jsonLayout; */
    /* std::stringstream ss; */
    ss << js;
        /* string word; */
        while(ss >> word){
            jsonLayout.append(word).append(string(" "));
        }
    /* ss >> jsonLayout; */
    cout << "a to str WebPageQuery::jsonlayoutNULLResultstring" << endl << jsonLayout<< endl;
    return jsonLayout;

}

void test01(){
    jsonLayOutNullResultString();
    json js = 
    {
        {"length", 1},
        {"type", 200},
        {"webarray", {}}
    };
    /* js["msgContent"].push_back( */
    /*                            { */
    /*                                 {"title", title}, */
    /*                                 {"summary", summary}, */
    /*                                 {"url", "www.wangdao.com"}, */
    /*                            } */
    /*                           ); */
}
int main(int argc, char* argv[])
{
   
       test01();
          return 0;
}
/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：19.cc
*   @author     ：anonymous
*   @date       ：2021年09月06日 01:48:29
*   @description：
*
================================================================*/

