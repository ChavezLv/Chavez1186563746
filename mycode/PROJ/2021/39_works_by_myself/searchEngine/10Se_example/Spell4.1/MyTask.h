#ifndef __MYTASK_H__
#define __MYTASK_H__

#include "TcpConnection.h"

#include<queue>
#include<string>
#include<vector>
#include <set>
#include<functional>


using std::set;
using std::vector;
using std::string;
using std::priority_queue;

namespace wd{

class Cache;
class CacheManage;
struct MyResult{
    MyResult(int dist,string mbword,int freq)
    :_distance(dist)
    ,_maybeWord(mbword)
    ,_frequence(freq)
    {

    }
    int _distance;
    string _maybeWord;
    int _frequence;

};
//求最小值用最大堆，每次pop出去的都是较大的值，这样留下的就是较小值了
//a<b吗? 小的话就返回true,因为要交换上去
struct Compare{
    

    bool operator()(const MyResult&lhs,const MyResult&rhs){
        if(lhs._distance>rhs._distance){
            return false;
        }
        else if(lhs._distance<rhs._distance){
            return true;
        }
        else{
            if(lhs._frequence>rhs._frequence){
                return false;
            }
            else if(lhs._frequence<rhs._frequence){
                return true;
            }
            else{
                return lhs._maybeWord<rhs._maybeWord;
            }
        }
       /* return lhs._distance>rhs._distance?true:lhs._distance<rhs._distance?false:lhs._frequence<rhs._frequence?true: */
       /*          lhs._frequence>rhs._frequence?false:lhs._maybeWord>rhs._maybeWord?true:false; */
    }
};

class MyTask
{
public:
    MyTask(TcpConnectionPtr,const string&,int,CacheManage&);
    ~MyTask() {}
    void execute();
    void statistic(set<int>&);
    int distance(const string&msg);
    void response();
private:
    string searchInCache();
    string searchInTable();
    int length(const string&);
    int nByte(const char&);
    string jsonString();
private:
    TcpConnectionPtr _conn;
    string _queuyword;
    priority_queue<MyResult,vector<MyResult>,Compare> _result;
    /* set<string> _occursWords; */
    size_t _resultSize;

    CacheManage & _cacheManage;
    Cache & _myCache;
};
}//end of namespace;
#endif
