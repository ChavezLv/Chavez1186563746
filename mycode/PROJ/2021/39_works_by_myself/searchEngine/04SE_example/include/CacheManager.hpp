#ifndef __CACHEMANAGER_HPP__
#define __CACHEMANAGER_HPP__

#include "Cache.hpp"

#include <vector>

using std::vector;

namespace spellCorrect
{

class CacheManager
{
public:
    static CacheManager* getInstance(size_t cacheSize,const string& filename);

    void init(size_t cacheSize,const string& filename);
    LRUCache& getCache(size_t idx);
    void periodicUpdateCaches();

    struct AutoRelease
    {
    public:
        ~AutoRelease();
    };
private:
    CacheManager() {}
    ~CacheManager() {}

private:
    static CacheManager* _pInstance;
    static AutoRelease _auto;
    vector<LRUCache> _cacheList;
};

}//end of namespace spellCorrect

#endif
