#include "../../include/CacheManager.hpp"

#include <iostream>

using std::cout;
using std::endl;

namespace spellCorrect
{
CacheManager* CacheManager::_pInstance = nullptr;
CacheManager::AutoRelease CacheManager::_auto;

CacheManager* CacheManager::getInstance(size_t cacheSize,const string& filename)
{
    if(!_pInstance)
    {
        _pInstance = new CacheManager;
        _pInstance->init(cacheSize,filename);
    }
    return _pInstance;
}

void CacheManager::init(size_t cacheSize,const string& filename)
{
    _cacheList.reserve(cacheSize);

    //初始化一个cache
    LRUCache cache;
    cache.init(filename);

    //赋值size个cache
    for(size_t i = 0; i < cacheSize; ++i)
    {
        _cacheList.push_back(cache);
    }
}

LRUCache& CacheManager::getCache(size_t idx)
{
    return _cacheList[idx];
}

void CacheManager::periodicUpdateCaches()
{
    int cnt = 0;
    for(auto cache: _cacheList)
    {
        cnt += cache.updateCount();
    }
    cout << cnt << "datas needed update" << endl;
    if(0 == cnt)
    {
        return;
    }
    //先将所有新数据汇总到0号cache
    for(size_t i = 1; i < _cacheList.size(); ++i)
    {
        _cacheList[0].update(_cacheList[i]);
    }
    //再依次更新其他cache
    for(size_t i = 1; i < _cacheList.size(); ++i)
    {
        _cacheList[i].update(_cacheList[0]);
    }

    //清空待更新数据
    for(auto cache: _cacheList)
    {
        cache.clearUpdateData();
    }
}

CacheManager::AutoRelease::~AutoRelease()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

}//end of namespace spellCorrect

