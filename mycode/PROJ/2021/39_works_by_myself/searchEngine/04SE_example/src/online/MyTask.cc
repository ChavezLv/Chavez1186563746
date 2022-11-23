#include "../../include/MyTask.hpp"
#include "../../include/EditDistance.hpp"
#include "../../include/MyDict.hpp"
#include "../../include/CacheManager.hpp"
#include "../../include/Thread.hpp"

#include <iostream>

#include "../../include/json/json.h"

using std::cout;
using std::endl;

namespace spellCorrect
{

bool MyCompare::operator()(const MyResult& lhs,const MyResult& rhs)
{
    if(lhs._iDist != rhs._iDist)
    {
        return lhs._iDist > rhs._iDist;
    }
    else if(lhs._iFreq != rhs._iFreq)
    {
        return lhs._iFreq > rhs._iFreq;
    }
    else
    {
        return lhs._word > rhs._word;
    }
}

MyTask::MyTask(const string& word,const TcpConnectionPtr& conn)
    :_queryWord(word)
     ,_conn(conn)
{}

void MyTask::execute()
{
    cout << "execute" << endl;
    CacheManager* cacheManager = CacheManager::getInstance(4,"../data/cache.dat");
    
    int threadName = static_cast<size_t>(*current_thread::name - '0');
    cout << "current thread: " << threadName << endl;

    LRUCache& cache = cacheManager->getCache(threadName);
    string retMsg = cache.get(_queryWord);
    if(retMsg == string())
    {
        cout << "cache miss" << endl;
        queryIndexTable();
        Json::Value root;
        Json::FastWriter writer;
        for(int i = 0; i < 5 ; ++i)
        {
            root[std::to_string(i)] = _resQue.top()._word;
            _resQue.pop();
        }

        retMsg = writer.write(root);
        cache.set(_queryWord,retMsg);
    }

    cout << "sendInLoop" << endl;
    _conn->sendInLoop(retMsg);
}

void MyTask::queryIndexTable()
{
    cout << "queryIndexTable" << endl;
    MyDict* pdict = MyDict::getInstance();
    auto indexTable = pdict->getIndex();
    for(int i = 0; i < _queryWord.size(); ++i)
    {
        int nBytes = nBytesCode(_queryWord[i]);
        string ch = _queryWord.substr(i,nBytes);
        cout << ch << endl;
        set<int> iset = indexTable[ch];
        statistic(iset);
        i += (nBytes -1);
    }
}

void MyTask::statistic(set<int>& iset)
{
    cout << "statistic" << endl;
    MyDict* pdict = MyDict::getInstance();
    auto dict = pdict->getDict();
    for(auto& idx: iset)
    {
        MyResult newElem;
        newElem._word = dict[idx].first;
        newElem._iFreq = dict[idx].second;
        newElem._iDist = distance(newElem._word);
        _resQue.push(newElem);
    }
}

int MyTask::distance(const string &rhs)
{
    return editDistance(_queryWord,rhs);
}

}//end of namespace spellCorrect

