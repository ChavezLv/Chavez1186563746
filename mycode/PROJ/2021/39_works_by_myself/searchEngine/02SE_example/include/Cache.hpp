#ifndef __CACHE_HPP__
#define __CACHE_HPP__
#include <unordered_map>
#include <string>
#include <list>
namespace wd
{

class Cache//缓存类
{
    using IterListPair=std::list<std::pair<std::string,std::string>>::iterator;
private:
    size_t _maxCache;
    std::unordered_map<std::string,IterListPair> _hashtable;//缓存
    std::list<std::pair<std::string,std::string>> _list;
public:
    Cache(size_t num=20);
    Cache(const Cache& cache);
    
    Cache &operator=(const Cache& cache);

    void addElement(const std::string &key,const std::string &val);//往缓存中添加数据
    void readFromFile(const std::string &filename);//从文件中读取缓存信息
    void writeToFile(const std::string &filename);//将缓存信息写入文件
    void update(const Cache &);//更新缓存信息,用到别人的hash覆盖自己的


    std::string hashFind(const std::string &word);

    std::string get(const std::string &key);
    void put(const std::string &key,const std::string &val);
};

}
#endif
