#ifndef __MYTASK_HPP__
#define __MYTASK_HPP__

#include "TcpConnection.hpp"
#include "MyResult.hpp"
#include "Cache.hpp"

#include <queue>
#include <vector>
#include <set>
using std::set;
using std::vector;
using std::priority_queue;

namespace spellCorrect
{

class MyCompare
{
public:
    bool operator()(const MyResult& lhs,const MyResult& rhs);
};

class MyTask
{
public:
    MyTask(const string& word,const TcpConnectionPtr& conn);
    
    void execute();
private:
    void queryIndexTable();
    void statistic(set<int>& iset);//从每个字符对应的set得到关联词
    int distance(const string& rhs);
    void response(LRUCache& cache);
private:
    string _queryWord;
    const TcpConnectionPtr& _conn;
    priority_queue<MyResult,vector<MyResult>,MyCompare> _resQue;
};


}//end of namespace spellCorrect

#endif
