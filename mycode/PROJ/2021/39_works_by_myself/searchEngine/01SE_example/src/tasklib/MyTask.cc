#include "../../libs/Mylog.hh"
#include "../../include/tasklib/MyTask.hh"
#include "../../include/tasklib/Mydict.hh"
#include "../../include/tasklib/MyResult.hh"
#include "../../include/cache/CacheManager.hh"
#include "../../include/tasklib/EditDistance.hh"
#include "../../include/networklib/TcpConnection.hh"
#include "../../include/locallib/Configuration.hh"
#include "../../include/networklib/Thread.hh"
#include "../../include/cache/CacheManager.hh"

#include <json/json.h>
#include <json/writer.h>

#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::string;
using std::map;
using std::set;
using std::unique_ptr;
using std::stringstream;

namespace wd
{

bool MyCompare::operator()(MyResult & lhs, MyResult & rhs)
{
    return lhs._iDist > rhs._iDist ? true :
        (lhs._iDist == rhs._iDist ? (lhs._iFreq > rhs._iFreq) : false);
}

size_t MyResultHash::operator()(MyResult const & res) const
{
    return std::hash<string>()(res._word);
}

bool MyResultEqual::operator()(MyResult const & lhs, MyResult const & rhs) const
{
    return lhs._word == rhs._word;
}


MyTask::MyTask(const std::string & queryWord, const TcpConnectionPtr & conn)
    : _queryWord(queryWord)
    , _conn(conn)
{}


void MyTask::execute()
{
    logInfo("active thread num : ", wd::current_thread::threadName);

    stringstream ss;

    Cache & cache = CacheManager::getInstance()->getCache(::atoi(wd::current_thread::threadName));

    ss << cache.getElement(_queryWord);

    // 如果没有在cache里找到的话
    if(ss.str().size() == 0) {
        queryIndexTable();

        Json::Value root;
        Json::StreamWriterBuilder builder;

        // 设定格式
        builder.settings_["indentation"] = "";

        const unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

        Configuration conf;
        size_t maxres = ::atol(conf.getConfigMap()["maxResNum"].c_str());

        for(auto k = _resultSet.begin(); k != _resultSet.end(); ++k) {
            _resultQue.push(*k);
        }

        for(size_t i = 0; i != maxres && !_resultQue.empty(); ++i) {
            root.append(_resultQue.top()._word);
            _resultQue.pop();
        }

        writer->write(root, &ss);
        // 保存到cache中
        cache.addElement(_queryWord, ss.str());
    }

    _conn->sendInLoop(ss.str());
}


void MyTask::queryIndexTable()
{
    Mydict * pMyDict = Mydict::createInstance(); 
    map<string, set<int>> & index = pMyDict->getIndexTable();

    // 为了提升效率
    set<int> avoid_multi;
    for(size_t idx = 0; idx != _queryWord.size(); ++idx) {
        int nBytes = nBytesCode(_queryWord[idx]);
        auto it = index.find(_queryWord.substr(idx, nBytes));
        if(it != index.end()) {
            set<int> stati;
            std::set_difference(it->second.begin(), it->second.end(),
                avoid_multi.begin(), avoid_multi.end(),
                std::inserter(stati, stati.begin()));

            statistic(stati);

            avoid_multi.insert(stati.begin(), stati.end());
        }
        idx += nBytes - 1;
    }
}

void MyTask::statistic(std::set<int> & iset)
{
    Mydict * pMyDict = Mydict::createInstance(); 
    vector<pair<string,int>> dict = pMyDict->getDict();

    MyResult res;
    size_t dist;

    for(auto i : iset) {
        res._word = dict[i].first;
        res._iFreq = dict[i].second;
        res._iDist = distance(res._word);
        /* logDebug("res >>> ", res._word, " ", res._iFreq, " ", res._iDist); */
        dist = res._iDist;
        if(dist < res._word.size()) {
            _resultSet.insert(res);
        }
    }
}

inline int triple_min(const int & a, const int & b, const int & c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int MyTask::distance(const std::string & rhs)
{
    size_t lhs_len = length(_queryWord);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for(size_t idx = 0; idx <= lhs_len; ++idx) {
        editDist[idx][0] = idx;
    }
    for(size_t idx = 0; idx <= rhs_len; ++idx) {
        editDist[0][idx] = idx;
    }

    std::string sublhs, subrhs;
    for(size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx) {
        size_t nBytes = nBytesCode(_queryWord[lhs_idx]);
        sublhs = _queryWord.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);

        for(size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx) {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += nBytes - 1;
            if(sublhs == subrhs) {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
            } else {
                editDist[dist_i][dist_j] = triple_min(
                    editDist[dist_i][dist_j-1] + 1,
                    editDist[dist_i - 1][dist_j] + 1,
                    editDist[dist_i - 1][dist_j - 1] + 1
                    );
            }
        }
    }
    return editDist[lhs_len][rhs_len];
}

void MyTask::response(Cache & cache)
{

}

} // end of namespace wd
