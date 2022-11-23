#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <map>
#include <string>

using std::map;
using std::string;

namespace spellCorrect
{
const string CONF = "dict.conf";

class Configuration
{
public:
    static Configuration* getInstance(const string& filepath);
    map<string,string>& getConfigMap();

    class AutoRelease
    {
    public:
        ~AutoRelease();
    };
private:
    Configuration(const string& filepath);
    ~Configuration() {}

    void buildMap(const string& filename);
private:
    string _filepath;//语料文件路径
    map<string,string> _configMap;//配置文件

    static Configuration* _pInstance;
    static AutoRelease _auto;
};



}//end of spellCorrect

#endif
