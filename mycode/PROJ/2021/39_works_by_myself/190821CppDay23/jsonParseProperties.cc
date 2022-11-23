#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <string>
#include <jsoncpp/json/json.h>
#include <fstream>
using std::fstream;

template<typename _Tp>
class Solution {
public:

};

void test01(){

    Json::Value root;
    Json::Value arrayObj;
    Json::Value item;

    item["C++"] = "jsonC++";
    item["Lua"] = "jsoninLua";
    item["Python"] = "support";
    arrayObj.append(item);

    root["name"] = "json";
    root["array"] = arrayObj;

    root.toStyledString();
    std::string out = root.toStyledString();
    std::cout << out << std::endl;

}
void test02(){
    Json::Value root;
    Json::FastWriter fast;

    root["ModuleType"] = Json::Value(1);
    root["ModuleCode"] = Json::Value("China");

    cout << fast.write(root) << endl;

    Json::Value sub;
    sub["version"] = Json::Value("1.0");
    sub["country"] = Json::Value(root);
    fast.write(sub);
    cout << sub.toStyledString() << endl;
}
void readAndPrint(){
    //开始读取
    fstream fsJson("json.properties", std::ios::in);
    if(!fsJson.good()){
        operator<<(cout, "no json.properties");
        return;
    }
    string strValue;
    string strLine;
    while(getline(fsJson, strLine)){
        strLine.pop_back();
        strValue = strValue + strLine; 
    }
    fsJson.close();
    
    //开始打印
    Json::Reader reader; //read
    Json::Value value; // value 可以是任意值
    if(reader.parse(strValue, value)){
        //name
        Json::Value out = value["name"].asCString();
        string name = value["name"].asCString();//用Json::Value接打印带""
        cout << name << endl;// Jackie
        cout << out << endl;// "Jackie"
        //age
        out = value["age"].asString();
        cout << out << endl;
        //language
        Json::Value outLan = value["languanges"];//const
        for(unsigned int i = 0; i < (unsigned int)outLan.size(); ++i){
            Json::Value outLanStr = outLan[i];
            cout << outLanStr << " ";
            if(i == (outLan.size() - 1)){
                cout << endl;
            }
        }

        //phone
        out = value["phone"].asString();//ok
        cout << out << endl;
        /* out = value["phone"];//ok */

        //book
        out = value["books"];
        for(auto &elem: out){
            Json::Value bookName = elem.begin().key().asString();
            Json::Value bookPrice = elem[bookName.asCString()].asDouble(); 
            cout << bookName << " " << bookPrice << endl;
        }

        //vip
        out = value["vip"].asBool();
        cout << out << endl;

        //address
        out = value["address"].asString();
        cout << out << endl;

    }

}
int main(int argc, char* argv[])
{
    readAndPrint();

    return 0;
}

