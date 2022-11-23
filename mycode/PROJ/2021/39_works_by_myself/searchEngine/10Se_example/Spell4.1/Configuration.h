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
    void getConfigMap();    //得到配置文件内容
    
    string getenDictPath();    //获得英文语料库路径
    /* void getcnDictPath();    //获得中文语料库路径 */
    string getaimsdir();     //获得生成词典路径

    void printfconfigmap(); //打印配置文件内容，测试用

/* private: */
    string _filepath;
    map<string,string> _configMap;
};

} //end of namespace yt

#endif

