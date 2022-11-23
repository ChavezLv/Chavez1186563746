/* <MyTask> -*- C++ -*- */
#ifndef __MYTASK_H__
#define __MYTASK_H__

#include "MyResult.hh"

#include <string>
#include <queue>
#include <vector>
#include <set>
#include <memory>
#include <unordered_set>



namespace wd
{

class Cache;

struct MyCompare
{
    bool operator()(MyResult & lhs, MyResult & rhs);
};

struct MyResultHash
{
    size_t operator()(MyResult const &) const;
};

struct MyResultEqual
{
    bool operator()(MyResult const & lhs, MyResult const & rhs) const;
};

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class MyTask
{
public:
    MyTask(const std::string & queryWord, const TcpConnectionPtr & conn);

    void execute();

private:
    void queryIndexTable();
    void statistic(std::set<int> & iset);
    int distance(const std::string & rhs);
    void response(Cache & cache);

private:
    std::string _queryWord;
    TcpConnectionPtr _conn;
    std::priority_queue<MyResult,
        std::vector<MyResult>,
        MyCompare> _resultQue;

    // 用于去重
    std::unordered_set<MyResult, MyResultHash, MyResultEqual> _resultSet;
};

} // end of namespace wd

#endif

