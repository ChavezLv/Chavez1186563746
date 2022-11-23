#ifndef __RXL_CACHEMANAGER_H__
#define __RXL_CACHEMANAGER_H__
#include"configuration.hpp"
#include"LRUcache.hpp"
#include<stdlib.h>
#include<vector>
using std::vector;
namespace rxl{
class Configuration;
class CacheManager{
public:
	static CacheManager* getInstance(Configuration& conf){
		if(nullptr == _pCache){
		    _pCache = new CacheManager(conf);
			atexit(destory);
		}
		return _pCache;
	}
	static void destory(){
		if(_pCache){
			delete _pCache;
			_pCache = nullptr;
		}
	}
	void init();
	void update();
	LRUCache& getCache(int i){return _caches[i];}
private:
	CacheManager(Configuration& conf)
	:_conf(conf)
	{}
private:
	vector<LRUCache> _caches;
	Configuration& _conf;
	static CacheManager *_pCache;
};
}//end of namespace
#endif
