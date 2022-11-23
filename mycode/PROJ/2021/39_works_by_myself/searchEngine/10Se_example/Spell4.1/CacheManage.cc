#include "CacheManage.h"
#include "Cache.h"
namespace  wd{
/* pthread_once_t CacheManage::_once=PTHREAD_ONCE_INIT; */
/* CacheManage* CacheManage::_instance=nullptr; */

void CacheManage::createCache(){
    for(int i=0;i<_num;++i){
        _cacheList.push_back(Cache(stoi(_conf.getMap()["cacheSize"])));
    }
}
Cache& CacheManage::getCache(){
    int idx=randInNum();
    
    /* MutexLockGuard mutex1(_mutex); */
    return _cacheList[idx];
}

int CacheManage::randInNum(){
    ::srand(::time(nullptr));
    return ::rand()%_num;
}
void CacheManage::periodicUpdateCache(){
    cout<<"periodicUpdateCache !"<<endl;
    
    //更新所有cache,得到最后的cache
    for(int i=1;i<_num;++i){
        _cacheList.begin()->update(_cacheList[i]);
    }
    //将最后的结果写入，每个cache
    for(int i=1;i<_num;++i){
        _cacheList[i]=_cacheList[0];
    }
    //将cache写入磁盘
    _cacheList.begin()->writeToFile(_conf.getMap()["cachePath"]);
}

void CacheManage::initFromFile(const string&filename){
    for(int i=0;i<_num;++i){
        _cacheList[i].readFromFile(filename);
    }
}

/* void CacheManage::initCacheManage(){ */
/*     _num=stoi(_conf.getMap()["ThreadpoolCap"]); */
/*     _threadtimer=ThreadTimer(std::bind(&CacheManage::periodicUpdateCache,this) */
/*                   ,stoi(_conf.getMap()["initTimer"]) */
/*                   ,stoi(_conf.getMap()["internalTimer"])); */

/*     createCache(); */
/*     _threadtimer.start(); */
        
/* } */
}//end of namespace
