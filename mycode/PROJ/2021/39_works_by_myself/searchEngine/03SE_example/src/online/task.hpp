#ifndef __RXL_TASK_H__
#define __RXL_TASK_H__
#include"cacheManager.hpp"
#include"tcpserver.hpp"
#include <ctype.h>
#include <string>
#include <cctype>
#include <queue>
#include <set>
#include <vector>
using std::set;
using std::vector;
using std::string;
using std::priority_queue;

namespace rxl{
class Configuration;
class DictProducer;
struct MyResult{
	MyResult(string word,int fre,int dis)
	:_word(word)
	,_fre(fre)
	,_dis(dis)
	{}
	string _word;
	int _fre;
	int _dis;
	bool operator==(const MyResult& rhs){
		return _word == rhs._word;
	}
	friend bool operator<(const MyResult& lhs,const MyResult& rhs);
	friend bool operator>(const MyResult& lhs,const MyResult& rhs);
	friend std::ostream& operator<<(std::ostream& os,const MyResult&);
};

class MyTask{
public:
	MyTask(const string& queryword, const TcpconnectionPtr& conn, DictProducer* pdic,CacheManager*);
	void response();
private:
	size_t nBytesCode(const char ch);
	string cleanupQuery(const string& str);
	void execute();
	void queryIndexTable();
	std::size_t length(const std::string &str);
	void statistic(string&,set<int> &,vector<std::pair<string,int>>&);
	int distance(const string & lhs,const string & rhs);
private:
	string _queryword;
	TcpconnectionPtr _conn;
	priority_queue<MyResult,vector<MyResult>,std::less<MyResult>> _queue;
	set<MyResult,std::greater<MyResult>> _resSet;
	DictProducer* _pdic;
	CacheManager* _pcache;
};
}//end of namespace
#endif
