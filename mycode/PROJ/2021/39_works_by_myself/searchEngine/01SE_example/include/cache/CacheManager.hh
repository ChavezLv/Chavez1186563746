#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__
#pragma once
#include "Cache.hh"
#include <vector>
#include <string>
using std::vector;
namespace wd
{
class CacheManager
{
public:
    CacheManager();
    ~CacheManager();
    static CacheManager * getInstance();
    static void destroy();
    /* 从磁盘文件中读取缓存信息 */
    void initCache(size_t cacheSize, const string &filename); 
    /* 获取某个缓存 */
    Cache & getCache(size_t idx);
    /* 定时更新所有的缓存 */
    void periodicUpdateCaches();

private:
    /* 单例模式 */
    static CacheManager * _pInstance; 
    vector<Cache> _cacheList;//n个Cache
    std::string _cacheFile;

};

}//end of namespace wd
#endif

