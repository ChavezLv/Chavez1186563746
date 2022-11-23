/* #t<<"recvCharainclude <iostream> */
#include <string>

#include <string>
#include <vector>
#include <fstream>
#include <cassert>
/* #include "json.hpp" */
#pragma comment(lib, "lib_json.lib")
#include "json/json.h"

using namespace::std;
int nByte(const char&c){
    int ans=1;
    if((c&(1<<7))){
        for(int i=0;i<=5;++i){
            int idx=6-i;
            if((c&(1<<idx))){
                ++ans; 
            }
            else{
                break;
            }
        }
    }
    return ans;
}

int length(const string&s){
    int n=0;
    for(int i=0;i<s.size();){
        i+=nByte(s[i]);
        ++n;
    }
    return n;
}
int dist(const string& _queuyword,const string&mbWord){
    int lenA =length(_queuyword);
    int lenB = length(mbWord);

    int d[lenA+1][lenB+1];


    for (int j = 0; j <= lenB; j++) {
        d[0][j]=j;

    }
    for (int i = 0; i <= lenA; i++) {
        d[i][0] = i;
    }
    cout<<"lenA = "<<lenA<<endl;
    cout<<"lenB = "<<lenB<<endl;
    string a,b;
    int idxA=0;
    for (int i = 1; i <= lenA; i++) {
        int curCharLengthA=nByte(_queuyword[idxA]);
        a=_queuyword.substr(idxA,curCharLengthA);
        idxA+=curCharLengthA;
        /* cout<<i<<"a = "<<a<<endl; */
        for (int j = 1,idxB=0; j <= lenB; j++) {
            int curCharLengthB=nByte(mbWord[idxB]);
            b=mbWord.substr(idxB,curCharLengthB);
            idxB+=curCharLengthB;
            /* cout<<j<<"b = "<<b<<endl; */
            if (a == b) {
                d[i][j] = d[i - 1][j - 1];
            } else {
                int m1 = d[i - 1][j] + 1;
                int m2 = d[i][j - 1] + 1;
                int m3 = d[i - 1][j - 1] + 1;
                d[i][j] =std::min(m1, m2);
                d[i][j]=std::min(d[i][j],m3);
            }
        }
    }
    return d[lenA][lenB];
}
void test0()
{
    string s="a可怜sdf中国";
    cout<<"size = "<<s.size()<<endl;
    /* cout<<"字符占据 "<<nByte(s[0])<<endl; */

    cout<<"共有字符 = "<<length(s)<<endl;

}
void test1(){
    /* cout<<"\"'The"<<endl; */
    cout<<"distance is "<<dist("hello","the")<<endl;
}
#if 1
void test2(){
    string strValue="{ \"the\":[\\\"the\\\",\\\"tha\\\",\\\"thb\\\"] }";
    cout << strValue << endl;
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(strValue, value))
    {
        cout << value["the"].asString() << endl;
        int size=value["the"].size();
        for(int i=0;i<size;i++){
            cout<<value["the"][i]<<" ";
        }
    }
}
//解包
void test3(){
    /* std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}"; */
    std::string strValue = "{\"array\":[\"tha\",\"thb\",\"thc\"],\"word\":\"the\n\"}";
    cout<<strValue<<endl;
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
        cout<<value["word"].asString()<<endl;
        int size=value["array"].size();
        for(int i=0;i<size;++i){
            cout<<value["array"][i].asString()<<endl;
        }
    }
}
//封包
void test4(){
    Json::Value arrayObj;
    Json::Value root;

    root["word"]="the";
    arrayObj.append("tha");
    arrayObj.append("thb");
    arrayObj.append("thc");

    root["arrry"]=arrayObj;

    cout<<root.asString()<<endl;
}

void test6(){
        ifstream ifs;
        ifs.open("checkjson.json");
        assert(ifs.is_open());
        
        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(ifs, root, false))
        {
            cerr << "parse failed \n";
            return;

        }
        string jsonStr=root.toStyledString();
        cout<< jsonStr<<endl;
        /* string name = root["name"].asString(); // 实际字段保存在这里 */
        /* int age = root["age"].asInt(); // 这是整型，转化是指定类型 */
        /* cout<<"name = "<<name<<endl; */
        /* cout<<"age = "<<age<<endl; */
}
#endif

#if 0
using json = nlohmann::json;
void test5(){
    map<string,vector<string>> m;
    m.insert({"word",{"the"}});
    m.insert({"ans",{"tha","thb","thc"}});

    json vec_j=m;

    cout<<vec_j.dump()<<endl;
    for(auto &p:vec_j){
        cout<<p<<endl;
    }
}
void test6(){
    json j;
    j["word"]="the";
    j["value"]={"tha","thc","thb"};

    cout<<j.dump()<<endl;
    for(auto &p:j.items()){
        cout<<p.key()<<" : "<<p.value()<<endl;
    }

}
#endif
void test7(string s){
    cout<<s<<endl;
    for(auto iter=s.begin();iter!=s.end();++iter){
        if(!isalpha(*iter)){
            s.erase(iter);
            --iter;
        }
    }
    cout<<s<<endl;
}
int main(){
    test7("ab3cd");
    test7("abcd");
    test7("123");
}

