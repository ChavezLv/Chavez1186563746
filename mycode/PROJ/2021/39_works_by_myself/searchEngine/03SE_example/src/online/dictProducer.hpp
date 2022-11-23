#ifndef __RXL_DICPRODUCER_H__
#define __RXL_DICPRODUCER_H__
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<unordered_map>
#include<fstream>
#include<sstream>
using std::set;
using std::unordered_map;
using std::pair;
using std::vector;
using std::map;
using std::string;
using std::cout;
using std::endl;
namespace rxl{

class Configuration;

class DictProducer{
public:
static DictProducer* getInstance(Configuration& conf){
	if(nullptr == _pDic){
		_pDic = new DictProducer(conf);
		atexit(destory);
	}
	return _pDic;
}
static void destory(){
	if(_pDic){
		delete _pDic;
		_pDic = nullptr;
	}
}
vector<pair<string,int> > & getEnDict();
unordered_map<string, set<int> > & getEnIndexTable();
vector<pair<string,int> > & getZhDict();
unordered_map<string, set<int> > & getZhIndexTable();

private:
DictProducer(Configuration&);
~DictProducer();
void init();
private:
	vector<pair<string,int>> _store_eng;
	vector<pair<string,int>> _store_cn;
	unordered_map<string,set<int>> _index_eng;
	unordered_map<string,set<int>> _index_cn;

	Configuration& _conf;
	static DictProducer* _pDic;
};
}//end of namespace
#endif
