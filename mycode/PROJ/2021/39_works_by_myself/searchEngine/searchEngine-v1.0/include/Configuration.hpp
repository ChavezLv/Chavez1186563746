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
#include <string>
#include <map>
#include <set>
using std::map;
using std::set;
using std::string;

namespace wd{

#define STOP_WORD_KEY "STOP_WORD_PATH"

class Configuration;
using ConfigurationPtr = Configuration *;


class Configuration{
    friend class Singleton<Configuration>;//私有的构造只能单例类使用了
private:
    Configuration(const string &filepath);
    void readConfiguration();

public:
    map<string, string> & getConfigMap();
    set<string> & getStopWordList();

private:
    string _filepath;//path of Configuration file
    map<string, string> _configMap;//content of Configuration file
    set<string> _stopWordList;//set of stop words

};

ConfigurationPtr configurationPtr = Singleton<Configuration>::getInstance("./conf/my.conf");

}//end of namespace wd

#endif //__WD_CONFIGURATION_HPP__
