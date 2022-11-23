#ifndef __CACHEMANAGE_H__
#define __CACHEMANAGE_H__
#include "Configuration.h"
#include "ThreadTimer.hpp"
#include "Cache.h"
/* #include "MutexLock.hpp" */
#include <pthread.h>


#include<vector>


using std::vector;
namespace wd{
class CacheManage
{
public:
    CacheManage(yt::Configuration& conf)
    :_conf(conf)
    ,_num(stoi(_conf.getMap()["ThreadpoolCap"]))
    ,_threadtimer(ThreadTimer(std::bind(&CacheManage::periodicUpdateCache,this)
                  ,stoi(_conf.getMap()["initTimer"])
                  ,stoi(_conf.getMap()["internalTimer"])))
    {   
        createCache();
        _threadtimer.start();
        initFromFile(_conf.getMap()["cachePath"]);
    }
    ~CacheManage() {}
    Cache& getCache();
    void initFromFile(const string&filename);
    void periodicUpdateCache();
#if 0
    static CacheManage*getInstance(){
        pthread_once(&_once,init);
        return _instance;
    }
    static void destroy(){
        if(_instance){
            delete _instance;
            _instance=nullptr;
        }
    }
    static void init(){
        if(_instance==nullptr){
            _instance=new CacheManage();
        } 
        ::atexit(destroy);
    }
    
    static void setConf(yt::Configuration& conf){
        _conf=conf;
    }
#endif
    /* void initCacheManage(); */

private:

    void createCache();
    int randInNum();
private:
    /* static pthread_once_t _once; */
    /* static CacheManage*_instance; */
    yt::Configuration&  _conf;
    
    int _num;
    vector<Cache> _cacheList;
    /* MutexLock _mutex; */
    ThreadTimer _threadtimer; 
    
};

}//end of namespace;
#endif

