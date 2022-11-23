#ifndef __CACHE_H__
#define __CACHE_H__
#pragma once
#include "../locallib/Configuration.hh"
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::list;
using std::unordered_map;
namespace wd
{
class Cache
{
public:
    using CacheNode = pair<string,string>;
    using iterator = list<CacheNode>::iterator;

    Cache(size_t capacity = 10);
    ~Cache();
    /* 从缓存中读取/删除数据 */
    /* get */
    string getElement(const string & key);
    /* 往缓存中添加数据 */
    /* set */
    void addElement(const string & key, const string & value);
    /* 从文件中读取缓存信息 */
    /* init */
    void readFromFile(const string & cachePath);
    /* 将缓存信息写入到文件中 */
    void writeToFile(string & cachePath);
    /* 获取待更新的记录数 */
    size_t getupdateCount() const;
    /* 更新缓存信息 */
    void update(const Cache & cache);
    /* 清空cache缓冲区 */
    void clearUpdateData();
private:
    /* 总记录数 */
    size_t _capacity;
    /* 待更新的记录数 */
    size_t _updateCount;
    /* 更新标识位,不需要更新为true,需要更新为false */
    bool _updateStatus;
    unordered_map<string, iterator> _cacheMap;
    /* 记录 */
    list<CacheNode> _dataList;
    /* 待更新的数据 */
    list<CacheNode> _pendingUpdateData;
};

}//end of namespace wd
#endif

