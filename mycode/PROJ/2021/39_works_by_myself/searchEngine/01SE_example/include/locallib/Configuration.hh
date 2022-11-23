/* <Configuration> -*- C++ -*- */
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>

namespace wd
{

class Configuration
{
public:
    Configuration(const std::string & filepath = _cfgFile);
    std::map<std::string, std::string> & getConfigMap()
    { return _configMap; }

private:
    // 配置文件路径
    std::string _filepath;
    // 配置文件内容
    std::map<std::string, std::string> _configMap;

    static const char * _cfgFile;

};

}
#endif

