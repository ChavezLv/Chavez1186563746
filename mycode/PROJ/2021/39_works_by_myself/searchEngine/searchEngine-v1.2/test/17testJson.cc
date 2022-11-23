/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：17testJson.cc
*   @author     ：anonymous
*   @date       ：2021年09月05日 19:59:56
*   @description：
*
================================================================*/

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
void test01(){
    json j =
      {
          {"pi", 3.141},
          {"happy", true},
          {"name", "Niels"},
          {"nothing", nullptr},
          {
              "answer", {
                  {"everything", 42}
              }
          },
          {"list", {1, 0, 2}},
          {
              "object", {
                  {"currency", "USD"},
                  {"value", 42.99}
              }
          }
      };

    cout << j  << endl;
       
}

void test02(){
    json j = 
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
    cout << j << endl;
    /* cout << j.at("/type"_json_pointer) << endl; //error*/
    /* cout << j.at("/webarray/1"_json_pointer) << endl; //error*/
    /* cout << j.at("/webarray/0"_json_pointer) << endl; //error */
    j["webarray"].push_back(

                        {
                            {"title", "z"},
                            {"summary", "def"},
                            {"url", "www.d.com"},
                        }
                            );
    cout << j["webarray"].size() << endl;
    /* int i = 3; */
    cout << j.at("/webarray/0"_json_pointer) << endl; //
    /* cout << j["webarrary"]["1"]["titile"] << endl; // error*/
    
    cout << "before to string " << endl;
    cout << j << endl;
    cout << "to string " << endl;
        string jsonLayout;
        std::stringstream ss;
        ss << j;
        ss >> jsonLayout;
        cout << jsonLayout << endl;

}
void test03(){
    json j = 
    {
            {
                {"title", "d"},
                {"summary", "def"},
                {"url", "www.d.com"},
            },
            {
                {"title", "d"},
                {"summary", "def"},
                {"url", "www.d.com"},
            },
            {
                {"title", "d"},
                {"summary", "def"},
                {"url", "www.d.com"},
            }
    };
    j.push_back(
            {
                {"title", "d"},
                {"summary", "def"},
                {"url", "www.d.com"},
            }
            );
    cout << j << endl;
}
void test04(){
    json js;
    /* js["a"] = 1; */
    cout << js << endl;
    /* js["a"].push_back("as"); */
    cout << js << endl;

}

int main(int argc, char* argv[])
{
   
       /* test01(); */
       test02();
       /* test03(); */
       test04();
          return 0;
}

