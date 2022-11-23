/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Configuration.cpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 17:10:21
*   @description：
*
================================================================*/


/* #include "WebPage.hpp" */
#include "Configuration.hpp"
#include "SplitTool.hpp"
#include <fstream>
#include <sstream>
using std::stringstream;
using std::ifstream;
using std::make_pair;//!= cppjieba::make_pair

namespace wd{

Configuration::Configuration(const string &filepath)
: _jieba(Singleton<SplitTool>::getInstance())
, _filepath(filepath){
    readConfiguration();
    loadDataFiles();
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

vector<pair<int, string> >& Configuration::getWebpages(){
    if(0 == _vecWebpages.size()){
        operator<<(cout, "Configuration::webpage file not exist");
    }
    return _vecWebpages;

}

unordered_map<int, pair<int, int> >& Configuration::getOffsetLib(){
    if(0 == _offsetLib.size()){
        operator<<(cout, "Configuration::offsetLib file not exist");
    }
    return _offsetLib;
}

unordered_map<string, set<pair<int, double> > >& Configuration::getInvertIndexTable(){
    if(0 == _invertIndexTable.size()){
        operator<<(cout, "Configuration::invertIndexTable file not exist");
    }
    return _invertIndexTable;
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
    cout << "Configuration: files data is ready!" << endl;
}

void Configuration::loadDataFiles(){
    ifstream ifsOffset(_configMap[NEW_WEBPAGE_OFFSET_LIB_PATH_KEY], std::ios::in);
    ifstream ifsWebpage(_configMap[NEW_WEBPAGE_LIB_PATH_KEY]);
    ifstream ifsInvert(_configMap[INVERT_INDEX_PATH_KEY]);
    if(!(ifsOffset.good() && ifsWebpage.good() && ifsInvert.good() ) ){
        cout << "WebPageQuery: data files error open!" << endl;
        return;
    }

    int  docId, offset, length;
    string line;
    int cnt = 0;
    while(getline(ifsOffset, line)){
        stringstream ss(line);
        ss >> docId >> offset >> length;

        _offsetLib.insert(make_pair(docId, make_pair(offset, length)));

        string doc;
        doc.resize(length, ' ');
        ifsWebpage.seekg(offset, ifsWebpage.beg);
        ifsWebpage.read(&*doc.begin(), length);
        /* SplitToolPtr cppjiebaPtr = Singleton<SplitTool>::getInstance(); */
        
        /* WebPage WebPage(doc, *this, *cppjiebaPtr); */
        /* _pageLib.insert(make_pair(docId, WebPage)); */
        _vecWebpages.push_back(make_pair(docId, doc));
    }
    ifsOffset.close();
    ifsWebpage.close();

    string word;
    double weight;
    stringstream ss;
    cnt = 0;
    while(getline(ifsInvert, line)){
        ss.clear();
        ss << line;
        set<pair<int, double> > setID;
        //逐个读取
        ss >> word;
        while(ss >> docId >> weight){
            setID.insert(make_pair(docId, weight));
        }
        _invertIndexTable.insert(make_pair(word, setID));
        
    }
    ifsInvert.close();

    cout << "WebPageQuery: data files ready!" << endl;

}


}//end of namespace wd

