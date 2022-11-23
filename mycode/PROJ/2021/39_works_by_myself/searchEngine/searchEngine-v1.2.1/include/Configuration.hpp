/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：Configuration.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 16:16:07
*   @description：
*
================================================================*/


#ifndef __WD_CONFIGURATION_HPP__
#define __WD_CONFIGURATION_HPP__

#include "TemplateSingleton.hpp"
/* #include "SplitTool.hpp" */
#include "WebPage.hpp"
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <memory>
using std::unordered_map;
using std::pair;
using std::unique_ptr;

using std::map;
using std::set;
using std::string;
using std::vector;

namespace wd{

#define STOP_WORD_KEY "STOP_WORD_PATH"
// add 
#define NEW_WEBPAGE_LIB_PATH_KEY "NEW_WEBPAGE_LIB_PATH" 
#define NEW_WEBPAGE_OFFSET_LIB_PATH_KEY "NEW_WEBPAGE_OFFSET_LIB_PATH"
#define INVERT_INDEX_PATH_KEY "INVERT_INDEX_PATH"

/* template<typename _Tp> */
/* class Singleton; */

class Configuration;
using ConfigurationPtr = Configuration *;
class SplitTool;
using SplitToolPtr = SplitTool*;

class WebPage;//only this is error for unordered_map


class Configuration{
    friend class Singleton<Configuration>;//私有的构造只能单例类使用了
private:
    Configuration(const string &filepath = "./conf/my.conf");//这里一个"." 是因为程序是在上一级目录运行的
    void readConfiguration();
    void loadDataFiles();

public:
    
    map<string, string> & getConfigMap();
    set<string> & getStopWordList();


    vector<pair<int, string> > &getWebpages(); // docId, doc
    unordered_map<int, WebPage > & getPageLib();
    unordered_map<int, pair<int, int> > & getOffsetLib();
    unordered_map<string, set<pair<int, double> > > &getInvertIndexTable();


private:
    SplitToolPtr _jieba;
    string _filepath;//path of Configuration file
    map<string, string> _configMap;//content of Configuration file
    set<string> _stopWordList;//set of stop words

    //for fast response to client, add 3 data member
    unordered_map<int, WebPage > _pageLib;
    vector<pair<int, string> > _vecWebpages;
    unordered_map<int, pair<int, int> > _offsetLib;
    unordered_map<string, set<pair<int, double> > > _invertIndexTable;
};


}//end of namespace wd

#endif //__WD_CONFIGURATION_HPP__
