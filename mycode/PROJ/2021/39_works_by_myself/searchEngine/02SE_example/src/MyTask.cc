#include "../include/MyTask.hpp"
#include "../include/Mydict.hpp"
#include "../include/CacheManger.hpp"
#include <string.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include <json/json.h>

using std::vector;
using std::pair;
using std::string;
using std::set;
using std::unordered_map;
using std::cout;
using std::endl;

namespace wd
{

Mydict *pInstance=Mydict::createInstance();

size_t nBytesCode(const char ch);
size_t length(const string &str);

inline int tripleMin(int a, int b, int c)
{
    return a < b ? (a < c ? a: c): (b < c? b: c);
}

void MyTask::execute()
{
    /* cout << "in execute" << endl; */
    /* cout << "_queryWord= " << _queryWord << endl; */
    unordered_map<string,set<int>> &indexTable = pInstance->getIndexTable();
    for(size_t idx=0; idx!=_queryWord.size(); ++idx){
        size_t nBytes = nBytesCode(_queryWord[idx]);
        /* cout << "nBytes= " << nBytes << endl; */
        string subtmp = _queryWord.substr(idx, nBytes);  
        /* cout << "subtmp = " << subtmp << endl; */
        statistic(indexTable[subtmp]); 

        idx += nBytes - 1;
    }

    cout << "exit execute" << endl;
}

/* void MyTask::queryIndexTable() */
/* { */
/*     //这void的咋查询，坑爹吧 */
/* } */

void MyTask::statistic(std::set<int> &iset)
{
    vector<pair<string,int>> &dict = pInstance->getDict();

    for(auto &idx: iset){

        MyResult tmp;
        tmp._word=dict[idx].first;
        tmp._iFreq=dict[idx].second;
        tmp._iDist=distance(dict[idx].first);
        /* cout << "tmp word= " << tmp._word << endl; */
        /* cout << "tmp freq= " << tmp._iFreq << endl; */
        /* cout << "tmp dist= " << tmp._iDist<< endl; */
        _resultQue.push(tmp);
    }

}

int MyTask::distance(const std::string &rhs)
{
    int len1, len2 ;
    len1 = length(_queryWord);
    len2 = length(rhs);

    vector<vector<int>> dp(len1+1,vector<int>(len2+1,0));
    //初始化
    for(int i=0; i!=len1+1; ++i){
        dp[i][0]=i;
    }
    for(int j=0; j!=len2+1; ++j){
        dp[0][j]=j;
    }
    //动态规划
    string sublhs, subrhs;
    for(int i=1, lhs_idx=0; i!=len1+1; ++i, ++lhs_idx){
        size_t nBytes = nBytesCode(_queryWord[lhs_idx]);
        sublhs = _queryWord.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes -1);

        for(int j=1, rhs_idx=0; j!=len2+1; ++j,++rhs_idx){
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes -1 );
            if(sublhs == subrhs){
                dp[i][j]=dp[i-1][j-1];
            }else{
                dp[i][j]=tripleMin(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])+1;
            }
        }
    }
    return dp[len1][len2];
}

void MyTask::response()
{
    CacheManger *cachemanger=CacheManger::getInstance();
    /* cout << "queueword= "<< _queryWord << endl; */
    //先找cache
    string result=cachemanger->getCache(pthread_self()).hashFind(_queryWord);
    if(result!=""){
        _conn->sendInt32(result.size());   
        /* _conn->send(result); */
        _conn->sendInLoop(result);
        return;
    }
    //没找到再从词典里找
    execute();
    
    int cnt =0;
    /* string msg; */

    MyResult lastResult;

    //用json封包
    Json::Value root;
    Json::FastWriter fwriter;
    string queryWord=_queryWord;
    root["queryWord"].append(queryWord.c_str());
    if(!_resultQue.empty()){
        lastResult=_resultQue.top(); 
        _resultQue.pop();
        /* cout << "word = " << lastResult._word << endl; */
        /* cout << "freq = " << lastResult._iFreq<< endl; */
        /* cout << "dist = " << lastResult._iDist<< endl; */
        /* cout << "word size= " << lastResult._word.size() << endl; */
        root["words"].append(lastResult._word.c_str());
        ++cnt;
    }
    while(!_resultQue.empty()&&cnt!=CANDIDIDATE_NUMBER){
        MyResult nowResult= _resultQue.top();
        _resultQue.pop();
        if(lastResult._word == nowResult._word){
            continue;
        }
        /* cout << "word = " << nowResult._word << endl; */
        /* cout << "freq = " << nowResult._iFreq<< endl; */
        /* cout << "dist = " << nowResult._iDist<< endl; */
        root["words"].append(nowResult._word.c_str());
        lastResult = nowResult;
        ++cnt;
    }
    string msg = fwriter.write(root);
    //存到缓冲中
    cachemanger->getCache(pthread_self()).addElement(_queryWord,msg);
    //发送结果
    _conn->sendInt32(msg.size());
    /* _conn->send(msg); */
    _conn->sendInLoop(msg);
}


size_t nBytesCode(const char ch)
{
    if(ch & (1<<7)){
        int nBytes =1;
        for(int idx=0; idx!=6; ++idx){
            if(ch & (1<<(6-idx))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

size_t length(const string &str)
{
    size_t ilen=0;
    for(size_t idx=0; idx!=str.size(); ++idx){
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

}//end of namespace wd
