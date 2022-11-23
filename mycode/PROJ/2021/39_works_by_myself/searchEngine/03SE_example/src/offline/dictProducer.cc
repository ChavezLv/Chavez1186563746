#include"splitTool.hpp"
#include"configuration.hpp"
#include"dictProducer.hpp"
rxl::DictProducer* rxl::DictProducer::_pDic = nullptr;
namespace rxl{
string DictProducer::cleanup_str_cn(string& source){
	int i = 0;
	string sourceWithOutSymbol;
	while (source[i] != 0)
	{
		if (source[i] & 0x80 && source[i] & 0x40 && source[i] & 0x20)
		{
			int byteCount = 0;
			if (source[i] & 0x10)
			{
				byteCount = 4;
			}
			else
			{
				byteCount = 3;
			}
			for (int a = 0; a < byteCount; a++)
			{
				sourceWithOutSymbol += source[i];
				i++;
			}
		}
		else if (source[i] & 0x80 && source[i] & 0x40)
		{
			i += 2;
		}
		else
		{
			i += 1;
		}
	}
	return sourceWithOutSymbol;
}
string DictProducer::cleanup_str(string& word){
	string ret;
	for(auto &c : word){
		if(isalpha(c)){
			ret += tolower(c);
		}
	}
	return ret;
}
void DictProducer::build_cn_dict()
{
	string word;
	for(auto p : _cn_file){
		std::ifstream fs(p.c_str());
		string text;
		if(!fs.good()){
			perror("fstream");
			continue;
		}   
		while(getline(fs,word)){
			text+=word;
		}   
		fs.close();
		vector<string> tmp = _tool.cut(text);
		for(auto s : tmp){
			s = cleanup_str_cn(s);
			if(s.size()){
				if(!_stopwords_cn.count(s)){
					++_cn_dic[s];
				}
			}
		}
	}
	auto itcn = _cn_dic.begin();
	while(itcn != _cn_dic.end()){
		_store_cn.push_back(*itcn);
		++itcn;
	}
}
void DictProducer::build_cn_index(){
	string source;	
	for(int idx = 0;idx < _store_cn.size();++idx){
		string source = _store_cn[idx].first;
		int i = 0;
		while(source[i] != 0){
			if (source[i] & 0x80 && source[i] & 0x40 && source[i] & 0x20)
			{
				int byteCount = 0;
				string index;
				if (source[i] & 0x10)
				{
					byteCount = 4;
				}
				else
				{
					byteCount = 3;
				}
				for (int a = 0; a < byteCount; a++)
				{
					index += source[i];
					i++;
				}
				_index_cn[index].insert(idx);
			}else{
				perror("build_cn_index");
				return ;
			}
		}
	}
}
void DictProducer::store_cn_index(const char* filepath){
	std::ofstream fs(filepath);
	if(!fs.good()){
		perror("ofstream");
		return;
	}
	for(auto s : _index_cn){
		fs<<s.first<<" ";
		for(auto i : s.second){
			fs<<i<<" ";
		}
		fs<<endl;
	}
	fs.close();
}
void DictProducer::build_cn_stopdic(const char* stopfilepath)
{
	std::ifstream fs(stopfilepath);
	if(!fs.good()){
		perror("ifstream");
	}
	string word;
	while(getline(fs,word)){
		_stopwords_cn.insert(word);
	}
	fs.close();
}
void DictProducer::store_cn_dict(const char* filepath)
{
	std::ofstream fs(filepath);
	if(!fs.good()){
		perror("ofstream");
		return;
	}
	auto it = _cn_dic.begin();
	while(it != _cn_dic.end()){
		fs<<it->first<<" "<<it->second<<endl;
		++it;
	}
}
void DictProducer::build_en_index(){
	string strc;
	strc.resize(1);
	for(int i = 0;i<_store_eng.size();++i){
		for(auto c : _store_eng[i].first){
			strc[0] = c;
			_index_eng[strc].insert(i);
		}
	}
}
void DictProducer::build_en_stopdic(const char* stopfilepath){
	std::ifstream fs(stopfilepath);
	if(!fs.good()){
		perror("ifstream");
	}
	string word;
	while(getline(fs,word)){
		word = cleanup_str(word);
		_stopwords_en.insert(word);
	}
	fs.close();
}
void DictProducer::build_en_dict(){
	string word;
	for(auto p : _file){
		std::ifstream fs(p.c_str());
		if(!fs.good()){
			perror("fstream");
			continue;
		}   
		while(getline(fs,word)){
			std::istringstream is(word);
			string tmp;
			while(is>>tmp){ 
				tmp = cleanup_str(tmp);
				if(tmp.size() == 0){
					continue;
				}
				else if(1 == _stopwords_en.count(tmp)){
					continue;
				}
				++_dic[tmp];
			}
		}   
		fs.close();
	}
	auto dicit = _dic.begin();
	while(dicit != _dic.end()){
		_store_eng.push_back(*dicit);
		++dicit;
	}
}
void DictProducer::store_en_dict(const char* filepath){
	std::ofstream fs(filepath);
	if(!fs.good()){
		perror("ofstream");
		return;
	}
	auto it = _dic.begin();
	while(it != _dic.end()){
		fs<<it->first<<" "<<it->second<<endl;
		++it;
	}
}
void DictProducer::store_en_index(const char* filepath){
	std::ofstream fs(filepath);
	if(!fs.good()){
		perror("ofstream");
		return;
	}
	for(auto s : _index_eng){
		fs<<s.first<<" ";
		for(auto i : s.second){
			fs<<i<<" ";
		}
		fs<<endl;
	}
	fs.close();
}
DictProducer::DictProducer(Configuration& conf,SplitTool& tool) 
:_conf(conf)
,_tool(tool)
{
	cout<<"DictProducer()"<<endl;
	string dire = _conf["english_yuliao_dir"].c_str();
	DIR *dir = opendir(dire.c_str());
	if(dir == nullptr){
		perror("opendir");
	}
	string tempdir = dire;
	tempdir+="/"; 
	struct dirent *pdir;
	while((pdir = readdir(dir))){       
		if(strcmp(pdir->d_name,".") == 0 || strcmp(pdir->d_name,"..") == 0)  
			continue;
		else{
			string abspath = tempdir + pdir->d_name;
			_file.push_back(abspath);
		}
	}
	closedir(dir);
	build_en_stopdic(_conf["english_stop_file"].c_str());
	build_en_dict();
	store_en_dict(_conf["english_store_file"].c_str());
	build_en_index();
	store_en_index(_conf["english_index_file"].c_str());
	
	string dirz = _conf["chinese_yuliao_dir"].c_str();
	dir = opendir(dirz.c_str());
	if(dir == nullptr){
		perror("opendir");
	}
	tempdir = dirz;
	tempdir+="/"; 
	while((pdir = readdir(dir))){       
		if(strcmp(pdir->d_name,".") == 0 || strcmp(pdir->d_name,"..") == 0)  
			continue;
		else{
			string abspath = tempdir + pdir->d_name;
			_cn_file.push_back(abspath);
		}
	}
	closedir(dir);
	build_cn_stopdic(_conf["chinese_stop_file"].c_str());
	build_cn_dict();	
	store_cn_dict(_conf["chinese_store_file"].c_str());
	build_cn_index();
	store_cn_index(_conf["chinese_index_file"].c_str());
}
DictProducer::~DictProducer(){ cout<<"~DictProducer()"<<endl; }
}//end of namspace
