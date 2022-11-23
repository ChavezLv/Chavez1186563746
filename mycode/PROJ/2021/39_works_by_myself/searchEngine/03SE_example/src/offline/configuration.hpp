#ifndef __RXL_CONFIGURATION_H__
#define __RXL_CONFIGURATION_H__
#include<stdio.h>
#include<errno.h>

#include<string>
#include<map>
#include<fstream>
#include<sstream>
#include<iostream>
using std::istringstream;
using std::ifstream;
using std::map;
using std::string;
using std::endl;
using std::cout;
namespace rxl{

class Configuration{
public:
	Configuration(string pathname)
	:_pathname(pathname)
	{
		ifstream ifs(_pathname.c_str());
		if(!ifs.good()){
			perror("ifstream");
		}
		string word,option,content;
		while(getline(ifs,word)){
			istringstream iss(word);
			iss>>option>>content;
			_configMap[option] = content;
		}
		ifs.close();
	}
	map<string,string> getConfigMap(){
		return _configMap;
	}
	string operator[](const string& str){
		return _configMap[str];
	}
private:
	string _pathname;
	map<string,string> _configMap;
};
}//end of namespace
#endif 
