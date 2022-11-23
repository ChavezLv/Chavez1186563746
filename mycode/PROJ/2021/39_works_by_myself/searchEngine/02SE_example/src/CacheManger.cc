#include "../include/CacheManger.hpp"
#include "../include/MyLog.hpp"
#include <iostream>
namespace wd
{

void CacheManger::init(size_t t)//从磁盘文件中读取缓存信息
{
    /*因为缓存数量和线程个数一致，所以t是线程数量*/
    for(size_t i=0;i!=t;++i)//0 ~ t-1
    {
        Cache cache;
        cache.readFromFile("../data/Cache.dat");
        _cacheList.push_back(std::make_pair(0,cache));
    }
}
Cache &CacheManger::getCache(size_t idx)//获取某个缓存,输入自己的线程id
{
    std::cout<<idx<<" 线程准备开始操作自己Cache"<<std::endl;
    auto it=_cacheList.begin();
    for(;it!=_cacheList.end();++it)
    {
        if(it->first==0)
        {
            it->first=idx;
            return it->second;
        }
        else if(it->first==idx)
        {
            return it->second;
        }
    }
    Cache cache;//其实不会退出循环,因为线程池创建只有一次，应该正好每个线程一个Cache不需要增加
    auto ret=_cacheList.insert(it,{idx,cache});//it=end(),相当于push_back,因为insert返回插入位置迭代器
    return ret->second;
}
void CacheManger::periodicUpdate()//定时更新所有缓存
{
    Cache cache;//临时cache，用来同步所有
    for(auto it=_cacheList.begin();it!=_cacheList.end();++it)
    {
        cache.update(it->second);//把所有线程的内容都存在临时Cache中
    }
    cache.writeToFile("../data/Cache.dat");//把这个存了所有Cache的内容,直接往文件写
    for(auto it=_cacheList.begin();it!=_cacheList.end();++it)
    {
        it->second.update(cache);//在把这个临时Cache的数据写到所有Cache中，完成同步全部Cache
    }
    logInfo("Cache has updated!");
}

}


