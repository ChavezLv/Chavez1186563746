#include "../../include/Cache.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace spellCorrect
{
LRUCache::LRUCache(size_t capacity)
    :_capacity(capacity)
     ,_updateCount(0)
     ,_updateStatus(false)
    {}

void LRUCache::init(const string& filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        std::cerr << filename << "ifs is not good" << endl;
    }

    string line,key,value;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        iss >> key >> value;
        set(key,value);
    }
    ifs.close();

    _updateStatus = true;
}

string LRUCache::get(const string& key)
{
    auto iter = _cacheMap.find(key);
    if(iter == _cacheMap.end())
    {
        return string();
    }
    else
    {
        _dataList.splice(_dataList.begin(),_dataList,iter->second);
        _cacheMap[key] = _dataList.begin();
        return _cacheMap[key]->second;
    }
}

void LRUCache::set(const string& key,const string& value)
{
    auto iter = _cacheMap.find(key);
    if(iter == _cacheMap.end())
    {
        //新结点
        if(_dataList.size() == _capacity)
        {
            //删除冷结点
            _cacheMap.erase(_dataList.back().first);
            _dataList.pop_back();
        }
        _dataList.push_front(make_pair(key,value));
        _cacheMap[key] = _dataList.begin();

        //如果是运行中，cache添加的新数据需要用于其他cache同步数据
        if(_updateStatus)
        {
            ++_updateCount;
            _pendingUpdateData.push_front(make_pair(key,value));
        }
    }
    else
    {
        if(_cacheMap[key]->second != value)
        {
            //更新数据
            _cacheMap[key]->second = value;
        }
        //热数据提前
        _dataList.splice(_dataList.begin(),_dataList,iter->second);
        _cacheMap[key] = _dataList.begin();
    }
}

size_t LRUCache::updateCount() const
{
    return _updateCount;
}

void LRUCache::update(LRUCache& cache)
{
    if(cache._pendingUpdateData.size() > 0)
    {
        for(auto& node:cache._pendingUpdateData)
        {
            set(node.first,node.second);
        }
    }
}

void LRUCache::clearUpdateData()
{
    _updateCount = 0;
    _pendingUpdateData.clear();
}

}//end of spellCorrect

#if 0
using namespace spellCorrect;

void testCache()
{
    LRUCache cache;
    cache.init("../data/cache.dat");
    string key;
    while(std::cin>>key)
    {
        cout << cache.get(key);
    }
}

int main()
{
    testCache();
    return 0;
}
#endif
