#ifndef __RXL_LRUCACHE_H__
#define __RXL_LRUCACHE_H__
#include<fstream>
#include<unordered_map>
#include<list>
#include<string>
#include<sstream>
using std::istringstream;
using std::ofstream;
using std::ifstream;
using std::string;
using std::unordered_map;
using std::list;
using std::pair;
#include<iostream>
using std::endl;
using std::cout;
namespace rxl{
class LRUCache {
public:
	LRUCache(int capacity)
	:_updatestate(false)
	,_capacity(capacity)
	,_updateNum(0)
	{}
	LRUCache(const LRUCache& rhs)
	:_updatestate(rhs._updatestate)
	,_capacity(rhs._capacity)
	,_updateNum(rhs._updateNum)
	,_cache(rhs._cache)
	,_updateCache(rhs._updateCache)
	{
		auto it = _cache.begin();
		while(it != _cache.end()){
			_map.insert(std::make_pair(it->first,it));
			++it;
		}
	}
	void initCache(string filepath){
		ifstream ifs(filepath);
		if(!ifs.good()){
			perror("fstream");
			return;
		}
		string word;
		string key;
		string value;
		while(getline(ifs,word)){
			istringstream iss(word);
			iss>>key>>value;
			put(key,value);
		}
		ifs.close();
		_updatestate = true;
	}

	string get(const string& key) {
		auto pos = _map.find(key);
		if(pos == _map.cend()){
			return string();
		}
		_cache.splice(_cache.begin(),_cache,pos->second);
		return _map[key]->second;
	}
	
	void put(string key, string value) {
		auto pos = _map.find(key);
		if(pos != _map.cend()){
			pos->second->second = value;
			_cache.splice(_cache.begin(),_cache,pos->second);
			return;
		}
		if(_cache.size() == _capacity){
			const auto& delpos = _cache.back();
			_map.erase(delpos.first);
			_cache.pop_back();
		}
		_cache.push_front(std::make_pair(key,value));
		_map[key] = _cache.begin();
		if(_updatestate){
			++_updateNum;
			_updateCache.push_front(std::make_pair(key,value));
		}
		return;
	}	   
	void clearupdate(){
		_updateNum = 0;
		_updateCache.clear();
	}
	void updateCache(LRUCache& cache){
		if(cache._updateCache.size() > 0){
			for(auto & uc : cache._updateCache){
				put(uc.first,uc.second);
			}
		}
	}
	int getUpdateNum() const {
		return _updateNum;
	}
	void storeInFile(ofstream& ofs){
		auto it = _cache.begin();
		while(it != _cache.end()){
			ofs<<it->first<<" "<<it->second;
			++it;
		}
	}
private: 
	bool _updatestate;
	//状态位
	int _capacity;
	int _updateNum;
	unordered_map<string,list<pair<string,string>>::iterator> _map;
	list<pair<string,string>> _cache;
	list<pair<string,string>> _updateCache;
};
}
#endif
