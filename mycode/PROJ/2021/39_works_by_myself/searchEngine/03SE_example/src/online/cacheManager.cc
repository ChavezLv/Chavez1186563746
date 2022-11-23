#include"cacheManager.hpp"
#include"configuration.hpp"
#include<functional>
#include<list>
#include<fstream>
using std::ofstream;
using std::list;
using std::bind;
namespace rxl{
CacheManager *CacheManager::_pCache = nullptr;
void CacheManager::init(){	
	
	int cacheNum = atoi(_conf["threadnum"].c_str());
	_caches.reserve(cacheNum);
	int capacity = atoi(_conf["cacheCapacity"].c_str());
	string filepath = _conf["cachefile"];
	LRUCache cache(capacity);
	cache.initCache(filepath);
	for(int i = 0;i<cacheNum;++i){
		_caches.push_back(cache);
	}
}

void CacheManager::update(){
	int count = 0;
	for(auto & cache : _caches){
		count+=cache.getUpdateNum();
	}
	if(!count){
		cout<<"无需更新"<<endl;
		return;
	}else{
		cout<<count<<"条记录待更新"<<endl;
	}
	for(int i = 1;i<_caches.size();++i){
		_caches[0].updateCache(_caches[i]);
	}

	for(int i  = 1;i<_caches.size();++i){
		_caches[i].updateCache(_caches[0]);
	}

	for(int i = 0;i<_caches.size();++i){
		_caches[i].clearupdate();
	}
	ofstream ofs(_conf["cachefile"]);
	if(!ofs.good()){
		perror("ofstream");
		return;
	}
	_caches[0].storeInFile(ofs);
	ofs.close();
}
}//end of namespace
