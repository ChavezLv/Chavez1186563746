#ifndef __RXL_DICPRODUCER_H__
#define __RXL_DICPRODUCER_H__
#include<stdio.h>
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
#include<unordered_set>
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

class SplitTool;

class DictProducer{
public:
static DictProducer* getInstance(Configuration& conf,SplitTool& tool){
	if(nullptr == _pDic){
		_pDic = new DictProducer(conf,tool);
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
private:
void build_en_dict();
void build_en_stopdic(const char*);
void build_en_index();
void build_cn_dict();
void build_cn_stopdic(const char*);
void build_cn_index();

void store_en_dict(const char*);
void store_en_index(const char*);
void store_cn_dict(const char*);
void store_cn_index(const char*);

string cleanup_str(string& word);
string cleanup_str_cn(string& source);
DictProducer(Configuration&,SplitTool&);
~DictProducer();
private:
	vector<string> _file;
	vector<string> _cn_file;
	map<string,int> _dic;
	map<string,int> _cn_dic;
	std::unordered_set<string> _stopwords_en;
	std::unordered_set<string> _stopwords_cn;

	vector<pair<string,int>> _store_eng;
	vector<pair<string,int>> _store_cn;
	unordered_map<string,set<int>> _index_eng;
	unordered_map<string,set<int>> _index_cn;

	Configuration& _conf;
	SplitTool& _tool;
	static DictProducer* _pDic;
};
}//end of namespace
#endif
