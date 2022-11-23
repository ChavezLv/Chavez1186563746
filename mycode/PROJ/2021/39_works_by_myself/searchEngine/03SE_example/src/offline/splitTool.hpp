#ifndef __RXL_SPLITTOOL_H__
#define __RXL_SPLITTOOL_H__
#include"configuration.hpp"
#include"../../include/cppjieba/Jieba.hpp"
#include<vector>
using std::vector;
namespace rxl{

class SplitTool{
public:
	SplitTool(Configuration& conf)
	:_conf(conf)
	,_jieba(_conf["DICT_PATH"].c_str(),_conf["HMM_PATH"].c_str(),
			_conf["USER_DICT_PATH"].c_str(),_conf["IDF_PATH"].c_str(),
			_conf["STOP_WORD_PATH"].c_str())
	{}
	vector<string>& cut(const string& str){	
		_jieba.CutForSearch(str, _words);
		return _words;
	}
private:
  Configuration& _conf;	
  cppjieba::Jieba _jieba;
  vector<string> _words;
};

}//end of namespace
#endif 
