#include"../../include/json/json.h"
#include"thread.hpp"
#include"LRUcache.hpp"
#include"dictProducer.hpp"
#include"task.hpp"
namespace rxl{
std::ostream& operator<<(std::ostream& os,const MyResult& res){
	os<<"( "<<res._word<<" , "<<res._fre<<" , "<<res._dis<<" )"<<endl;
	return os;
}

bool operator>(const MyResult& lhs,const MyResult& rhs){
	string lw,rw;
	if(lhs._dis == rhs._dis){	
		if(lhs._fre == rhs._fre){	
			lw = lhs._word;
			rw = rhs._word;
		}else{
			return lhs._fre>rhs._fre;
		}
	}else{
		return lhs._dis<rhs._dis;
	}
	return lw<rw;
}
bool operator<(const MyResult& lhs,const MyResult& rhs){
	string lw,rw;
	if(lhs._dis == rhs._dis){	
		if(lhs._fre == rhs._fre){	
			lw = lhs._word;
			rw = rhs._word;
		}else{
			return lhs._fre<rhs._fre;
		}
	}else{
		return lhs._dis>rhs._dis;
	}
	return lw>rw;
}
	
MyTask::MyTask(const string& queryword, const TcpconnectionPtr& conn,DictProducer* pdic,CacheManager* pcache)
:_queryword(queryword)
,_conn(conn)
,_pdic(pdic)
,_pcache(pcache)
{}
size_t MyTask::nBytesCode(const char ch)
{
	if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}  

std::size_t MyTask::length(const std::string &str)
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}

int MyTask::distance(const string& word1, const string& word2) {  
	int l1 = length(word1);
	int l2 = length(word2);
	vector<vector<int>> d(l1+1,vector<int>(l2+1,0));
	for(int i = 0;i<=l1;++i)d[i][0] = i;
	for(int j = 0;j<=l2;++j)d[0][j] = j;
	for(int i = 1,str1_i = 0;i<=l1;++i,++str1_i){
		int bytelen1 =nBytesCode(word1[str1_i]);
		string sub1 = word1.substr(str1_i,bytelen1);
		str1_i+=(bytelen1-1);
		for(int j = 1,str2_i = 0;j<=l2;++j,++str2_i){
			int bytelen2 =nBytesCode(word2[str2_i]);
			string sub2 = word2.substr(str2_i,bytelen2);
			str2_i+=(bytelen2-1);
			int c = (sub1 == sub2)?0:1;
			d[i][j] = std::min(d[i-1][j-1]+c,std::min(d[i][j-1],d[i-1][j])+1);
		}
	}
	return d[l1][l2];
}
void MyTask::response(){
	string qstr = cleanupQuery(_queryword);
	qstr.resize(qstr.size()-1);
	cout<<"这是"<<currentThread::threadName<<"号线程"<<endl;
	LRUCache& Cache = _pcache->getCache(atoi(currentThread::threadName));
	string result = Cache.get(qstr);
	if(result.size()){
		cout<<"通过缓存得到结果!"<<endl;
		_conn->sendInloop(result);
	}else{
		cout<<"无法通过缓存得到结果!"<<endl;
		execute();
		Json::Value root;
		Json::FastWriter writer;
		int count = 0;
		auto it = _resSet.begin();
		while(it != _resSet.end()){
			Json::Value person;
			person["word"] = it->_word;
			person["fre"] = it->_fre;
			person["dis"] = it->_dis;
			root.append(person);
			++count;
			if(count>10)break;
			++it;
		}
		string jsonres = writer.write(root);	
		Cache.put(qstr,jsonres);
		_conn->sendInloop(jsonres);
	}
}
string MyTask::cleanupQuery(const string& str){
	string res;
	for(auto s : str){
		if(!ispunct(s) && !isspace(s) && !isdigit(s)){
			res+=s;
		}
	}
	return res;
}
void MyTask::execute(){	
	vector<pair<string,int>> eng_dict;
	unordered_map<string,set<int>> eng_index;
	vector<pair<string,int>> cn_dict;
	unordered_map<string,set<int>> cn_index;
	cn_dict = _pdic->getZhDict();
	cn_index = _pdic->getZhIndexTable();
	eng_dict = _pdic->getEnDict();
	eng_index = _pdic->getEnIndexTable();
	//去除符号和数字
	string query = cleanupQuery(_queryword);
	query.resize(query.size()-1);
	cout<<"query = "<<query<<" size = "<<query.size()<<endl;
	int i = 0;
	while(query[i]){
		int bytelen = nBytesCode(query[i]);
		if(bytelen == 1){
			string idx = query.substr(i,bytelen);
			statistic(query,eng_index[idx],eng_dict);
			i+=bytelen;
		}else if(bytelen == 3){
			string idx = query.substr(i,bytelen);
			statistic(query,cn_index[idx],cn_dict);
			i+=bytelen;
		}
	}	
	while(!_queue.empty()){
		_resSet.insert(_queue.top());
		_queue.pop();
	}
}
void MyTask::statistic(string& queryword,set<int> & iset,vector<pair<string,int>>& dict){
	auto it = iset.begin();
	while(it != iset.end()){
		string word = dict[*it].first;
		int fre = dict[*it].second;
		int dis = distance(queryword,word);
		MyResult res(word,fre,dis);
		_queue.push(res);
		++it;
	}
}

}//end of namspace
