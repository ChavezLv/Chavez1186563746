#ifndef __YT_CONFIGURATION_HH__
#define __YT_CONFIGURATION_HH__


#include <iostream>
#include <fstream>
#include <string>
#include <map>

using std::string;
using std::map;
using std::ifstream;
using std::cout;
using std::endl;

namespace yt
{
class Configuration
{
public:
    Configuration(const string &filepath);
    map<string,string>& getMap(){return _configMap;}

    /* map<string,string> &getConfigMap(); */

    //自己设置

    //得到配置文件内容
    void getConfigMap();

    //获得英文语料库路径
    string getenDictPath();

    //获得中文语料库路径
    string getcnDictPath();

    //获得生成词典路径
    string getaimsdir();

    //获得索引文件路径
    string getindexdir();

    //获得停用词文件路径
    string getstopdir();

    //打印配置文件内容，测试用
    void printfconfigmap(); 

    /* private: */
    string _filepath;
    map<string,string> _configMap;
};

} //end of namespace yt

#endif

