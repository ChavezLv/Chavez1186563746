/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Configuration.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 17:10:21
*   @description：
*
================================================================*/


#include "Configuration.hpp"
#include <fstream>
#include <sstream>
using std::ifstream;

namespace wd{

Configuration::Configuration(const string &filepath)
: _filepath(filepath){
    readConfiguration();
}

map<string, string>& Configuration::getConfigMap(){
    return _configMap;
}

set<string>& Configuration::getStopWordList(){
    if(0 == _stopWordList.size()){
        string stopWordFilePath = _configMap[STOP_WORD_KEY];//后面可以补上, 没有判断
        std::ifstream ifs(stopWordFilePath);
        if(!ifs){
            operator<<(cout, "Configuration file not exist");
        }
        string word;
        while(getline(ifs, word)){
            _stopWordList.insert(word);
        }
    }
    return _stopWordList;
}

void Configuration::readConfiguration(){
    std::ifstream ifs(_filepath, std::ios::in);        
    if(!ifs.good()){
        operator<<(cout, "Configuration file not exist");
        return ;
    }

    string line;
    string key;
    string value;
    std::stringstream ss;
        cout << _filepath << endl;
    while(getline(ifs, line)){
        ss.clear();
        ss << line;
        ss >> key >> value;
        cout << "key: " << key << " value : " << value << endl;
        _configMap[key] = value;
    }
    cout << "Configuration file data is ready!" << endl;
}



}//end of namespace wd

