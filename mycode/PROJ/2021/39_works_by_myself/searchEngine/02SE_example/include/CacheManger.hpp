#ifndef __CACHEMANGER_HPP__
#define __CACHEMANGER_HPP__
#include "Cache.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
namespace wd
{

class CacheManger
{
private:
    std::vector<std::pair<size_t,Cache>> _cacheList;//缓存的数量与线程个数一致,size_t存线程号
    CacheManger(){};
    CacheManger(const CacheManger&)=delete;
    CacheManger& operator=(const CacheManger&)=delete;
public:
    void init(size_t t);//从磁盘文件中读取缓存信息
    Cache &getCache(size_t idx);//获取某个缓存,输入自己的线程id
    void periodicUpdate();//定时更新所有缓存

    static CacheManger* getInstance()//单例模式创建实例 
    {
                static CacheManger instance;
                return &instance;
    }
};//反正全家静态，就是个静态类嘛，直接声明定义
}
#endif
