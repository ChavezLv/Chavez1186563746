/* #include "CacheManager.hpp" */
#include "../../libs/Mylog.hh"
#include "../../include/cache/CacheManager.hh"
namespace wd
{
CacheManager *  CacheManager::_pInstance = nullptr;
CacheManager::CacheManager()
{}
CacheManager::~CacheManager()
{}
CacheManager * CacheManager::getInstance()
{
    if(nullptr == _pInstance){
        _pInstance = new CacheManager();
    }
    return _pInstance;
}
void CacheManager::destroy()
{
    if(_pInstance){
        delete _pInstance;
    }
}
void CacheManager::initCache(size_t cacheSize, const string &filename)
{
    _cacheFile = filename;
    _cacheList.reserve(cacheSize);
    Cache cache;
    cache.readFromFile(filename);
    logDebug("cacheSize = ", cacheSize);
    for(size_t idx = 0; idx< cacheSize;++idx){
        _cacheList.push_back(cache);
    }
}
Cache & CacheManager::getCache(size_t idx)
{
    return _cacheList[idx];
}
void CacheManager::periodicUpdateCaches()
{
    size_t cnt = 0;
    logInfo("updateCache");
    for(auto & cache : _cacheList){
        cnt += cache.getupdateCount();
    }
    if(cnt == 0){
        logInfo("no need to update");
        return ;
    }
    else{
        logInfo("There are ", cnt, " records that need to be updated");
    }


    for(size_t idx = 1; idx!= _cacheList.size();++idx){
        _cacheList[0].update(_cacheList[idx]);
    }
    
    logDebug("CacheManager merge all to idx 0");

    for(size_t idx = 1;idx != _cacheList.size();++idx){
        _cacheList[idx].update(_cacheList[0]);
    }
    for(size_t idx = 0; idx != _cacheList.size();++idx){
        _cacheList[idx].clearUpdateData();
    }

    logDebug("CacheManager merge cache");

    _cacheList[0].writeToFile(_cacheFile);

    logDebug("CacheManager write to file");
}
}//end of namespace wd
