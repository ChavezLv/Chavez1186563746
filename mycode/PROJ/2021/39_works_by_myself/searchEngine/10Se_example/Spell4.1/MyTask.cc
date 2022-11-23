#include "MyTask.h"
#include "Mydict.h"
#include "CacheManage.h"
#include "Cache.h"
#include <algorithm>
#include <iostream>

#pragma comment(lib, "lib_json.lib")
#include "json/json.h"
/* #include "nlohmann/json.hpp" */
using std::cout;
using std::endl;
namespace wd{

MyTask::MyTask(TcpConnectionPtr conn,const string& msg,int k,CacheManage &cacheManage)
:_conn(conn)
,_queuyword(msg)
/* ,_queuyword(msg.substr(0,msg.size()-1)) */
,_resultSize(k)
,_cacheManage(cacheManage)
,_myCache(cacheManage.getCache())
{
        
}

string MyTask::jsonString(){
    Json::Value arrayObj;
    Json::Value root;

    root["word"]=_queuyword;
    while(!_result.empty()){
        arrayObj.append(_result.top()._maybeWord);
        _result.pop();
    }

    root["array"]=arrayObj;

    return root.toStyledString();
}
string MyTask::searchInCache(){
    return _myCache.getElement(_queuyword);
}
string MyTask::searchInTable(){
    _occursWords.clear();
    auto idxTable=Mydict::getInstance()->getIndexTable();
    
    int idx=0;
    cout<<"SearchInTable !"<<_queuyword<<endl;
    cout<<"queryword has 字符 = "<<length(_queuyword)<<endl;
    cout<<"一二三四五六七八 字符 = "<<length("一二三四五六七八")<<endl;
    for(int i=0;i<length(_queuyword);++i){
        int curSize=nByte(_queuyword[idx]);

        cout<<"curSize = "<<curSize<<endl;
        string s=_queuyword.substr(idx,curSize);
        idx+=curSize;
        /* cout<<s<<endl; */
        /* cout<<idxTable[s].size()<<endl; */
        /* cout<<"cur char is "<<s<<endl; */
        cout<<"s = "<<s<<endl;
        statistic(idxTable[s]);
    }
    return jsonString();
}
void MyTask::execute(){
    cout<<"executing !"<<endl;
    //在缓存中查找
    string jsonStr=searchInCache();
    if(!jsonStr.empty()){
        cout<<"-------find in Cache !-------"<<endl;
        cout<<"jsonMsg is "<<jsonStr<<endl;
        _conn->sendInloop(jsonStr);
    }
    else{
    //在索引表里查找 并 放到缓存中
    jsonStr=searchInTable();
    _myCache.addElement(_queuyword,jsonStr);
    cout<<"---------find in Table!-----------"<<endl;
    
    //发送给客户端
    cout<<"jsonMsg is "<<jsonStr<<endl;
    _conn->sendInloop(jsonStr);
    }
}
  

void MyTask::statistic(set<int>& mbpos){
    cout<<"statisticing !"<<endl;
    auto dir=Mydict::getInstance()->getDict(); 
    cout<<"cur char has "<<mbpos.size()<<endl;
    for(auto &p:mbpos){
        /* cout<<"in  for !"<<endl; */
        int distan=distance(dir[p].first);
        
        MyResult mbResult(distan , dir[p].first , dir[p].second);  
        if(_occursWords.find(mbResult._maybeWord)==_occursWords.end()){
            _occursWords.insert(mbResult._maybeWord);
            _result.push(mbResult);
            if(_result.size()>_resultSize){
                _result.pop();
            }
        }
        /* cout<<"_result size "<<_result.size()<<endl; */
    }

}
int MyTask::nByte(const char&c){
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
int MyTask::length(const string&s){
    int n=0;
    for(int i=0;i<s.size();){
        i+=nByte(s[i]);
        ++n;
    }
    return n;
}
int MyTask::distance(const string&mbWord){
    /* cout<<mbWord<<endl; */
    int lenA =length(_queuyword);
    int lenB = length(mbWord);
    
    int d[lenA+1][lenB+1];
    
    
    for (int j = 0; j <= lenB; j++) {
        d[0][j]=j;

    }
    for (int i = 0; i <= lenA; i++) {
        d[i][0] = i;
    }

    string a,b;
    int idxA=0;
    for (int i = 1; i <= lenA; i++) {
        int curCharLengthA=nByte(_queuyword[idxA]);
        a=_queuyword.substr(idxA,curCharLengthA);
        idxA+=curCharLengthA;

        for (int j = 1,idxB=0; j <= lenB; j++) {
            int curCharLengthB=nByte(mbWord[idxB]);
            b=mbWord.substr(idxB,curCharLengthB);
            idxB+=curCharLengthB;
            
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
}//end of namespace;
