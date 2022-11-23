#ifndef __CACHE_HPP__
#define __CACHE_HPP__

#include <unordered_map>
#include <string>
#include <list>
#include <map>

using std::unordered_map;
using std::string;
using std::list;
using std::pair;

namespace spellCorrect
{
using CacheNode = pair<string,string>;
using iterator = list<CacheNode>::iterator;

class LRUCache
{
public:
    LRUCache(size_t capacity = 1000);

    void init(const string& filename);
    string get(const string& key);
    void set(const string& key,const string& value);
    size_t updateCount() const;
    void update(LRUCache& cache);
    void clearUpdateData();

private:
    size_t _capacity;//cache容量
    size_t _updateCount;//待更新数据数
    bool _updateStatus;
    unordered_map<string,iterator> _cacheMap;
    list<CacheNode> _dataList;
    list<CacheNode> _pendingUpdateData;
};

}//end of namespace spellCorrect

#endif
