#include "../include/json/json.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

void readStrJson()
{
    //字符串  
    const char* str =
        "{\"name\":\"shuiyixin\",\"age\":\"21\",\"sex\":\"man\"}";


    Json::Reader reader;
    Json::Value root;

    //从字符串中读取数据  
    if (reader.parse(str, root))
    {
        string name = root["name"].asString();
        int age = root["nomen"].asInt();
        string sex = root["sex"].asString();
        cout << name + "," << age << "," << sex <<  endl;
    }
}

void test()
{
    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;

    for (int i = 0; i < 10; i ++)
    {
        item["key"] = i;
        arrayObj.append(item);

    }

    root["key1"] = "value1";
    root["key2"] = "value2";
    root["array"] = arrayObj;
    //root.toStyledString();
    std::string out = root.toStyledString();
    std::cout << out << std::endl;
}

int main()
{
    test();
    return 0;
}

