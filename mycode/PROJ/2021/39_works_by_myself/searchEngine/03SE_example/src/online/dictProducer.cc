#include"configuration.hpp"
#include"dictProducer.hpp"
rxl::DictProducer* rxl::DictProducer::_pDic = nullptr;
namespace rxl{
DictProducer::DictProducer(Configuration& conf)
:_conf(conf)
{
	init();
}
DictProducer::~DictProducer(){
	cout<<"~DictProducer()"<<endl;
}

vector<pair<string,int> >& DictProducer::getEnDict(){
	return _store_eng;
}
unordered_map<string, set<int> >& DictProducer::getEnIndexTable(){
	return _index_eng;
}
vector<pair<string,int> >& DictProducer::getZhDict(){
	return _store_cn;
}
unordered_map<string, set<int> >& DictProducer::getZhIndexTable(){
	return _index_cn;
}
void DictProducer::init(){
	//保存英文词典
	std::fstream fs(_conf["english_store_file"].c_str());
	if(!fs.good()){
		perror("fstream");
	}
	string word;
	while(getline(fs,word)){
		istringstream iss(word);
		string key;
		int fre;
		iss>>key>>fre;
		_store_eng.push_back(std::make_pair(key,fre));
	}
	fs.close();
	//保存中文词典
	std::fstream fsc(_conf["chinese_store_file"].c_str());
	if(!fsc.good()){
		perror("fstream");
	}
	while(getline(fsc,word)){
		istringstream issc(word);
		string key;
		int fre;
		issc>>key>>fre;
		_store_cn.push_back(std::make_pair(key,fre));
	}
	fsc.close();
	//保存英文索引
	std::fstream fsei(_conf["english_index_file"].c_str());
	if(!fsei.good()){
		perror("fstream");
	}
	string indexcontent;
	while(getline(fsei,indexcontent)){
		istringstream issi(indexcontent);
		string ch;	
		int idx;
		issi>>ch;
		_index_eng[ch];
		while(issi>>idx){
			_index_eng[ch].insert(idx);
		}
	}
	fsei.close();
	//保存中文索引
	std::fstream fsec(_conf["chinese_index_file"].c_str());
	if(!fsec.good()){
		perror("fstream");
	}
	while(getline(fsec,indexcontent)){
		istringstream issi(indexcontent);
		string ch;	
		int idx;
		issi>>ch;
		_index_cn[ch];
		while(issi>>idx){
			_index_cn[ch].insert(idx);
		}
	}
	fsec.close();
}
}//end of namespace
