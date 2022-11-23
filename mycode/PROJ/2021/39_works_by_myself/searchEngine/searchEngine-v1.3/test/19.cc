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
        {"msgContent",// {}
            {
                        {//"1",// 没有用
                            {"title", "b"},
                            {"summary", "bcd"},
                            {"url", "www.b.com"}
                        },
                        {
                            {"title", "c"},
                            {"summary", "cde"},
                            {"url", "www.c.com"},
                        },
                        {
                            {"title", "d"},
                            {"summary", "def"},
                            {"url", "www.d.com"},
                        }
                    } 
        }
    };

    //改名字
    /* js.at("/webarray"_json_pointer) = "msgContent"; */

    js["msgContent"].push_back(
                               {
                                    {"title", title},
                                    {"summary", summary},
                                    {"url", "www.wangdao.com"},
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

    // 只打印了一个的大小， 就是第一个元素
    cout << endl << "size" << js["msgContent"].size() << "只打印了一个的大小， 就是第一个元素" << endl;
    json j_list_of_pairs = js["msgContent"];
    cout << "size: " << j_list_of_pairs.size() << " "<< j_list_of_pairs << endl;

    // 
    cout << endl << "json::iterator迭代器的使用" << endl;
    json::iterator j_iterator  =js["msgContent"].begin();
    for( ; j_iterator != js["msgContent"].end(); ++j_iterator){
        cout << *j_iterator << endl;// 也是打印一个元素
    }
    

    cout << endl;


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

