#include "../../include/Configuration.hpp"

#include <unistd.h>

#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;

namespace spellCorrect
{

Configuration* Configuration::_pInstance = nullptr;//这里不用考虑多线程问题
Configuration::AutoRelease Configuration::_auto;

Configuration* Configuration::getInstance(const string& filepath)
{
    if(!_pInstance)
    {
        _pInstance = new Configuration(filepath);
    }
    return _pInstance;
}

map<string,string>& Configuration::getConfigMap()
{
    return _configMap;
}

Configuration::Configuration(const string& filepath)
    :_filepath(filepath)
{
    buildMap(filepath+CONF);
}

void Configuration::buildMap(const string& filename)
{
    cout << getcwd(NULL,0) << endl;
    cout << filename << endl;
    ifstream ifs(filename);
    if(!ifs.good())
    {
        cerr << "ifs is not good" << endl;
    }
    while(ifs)
    {
        string key,value;
        ifs >> key >> value;
        _configMap[key] = value;
    }
}

Configuration::AutoRelease::~AutoRelease()
{
    if(Configuration::_pInstance)
    {
        delete Configuration::_pInstance;
        Configuration::_pInstance = nullptr;
    }
}


}//end of namespace spellCorrect
#if 0
using namespace spellCorrect;
const string PATH = "../conf/";
void testConfiguration()
{
    Configuration::getInstance(PATH);
    Configuration* pconf = Configuration::getInstance(PATH);
    map<string,string> conf = pconf->getConfigMap();
    for(auto& elem:conf)
    {
        cout << elem.first << " " << elem.second << endl;
    }

}
#endif
