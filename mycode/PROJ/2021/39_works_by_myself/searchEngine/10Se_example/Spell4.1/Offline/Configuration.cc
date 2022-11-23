#include "Configuration.h" 


namespace yt
{
Configuration::Configuration(const string &filepath)
    :_filepath(filepath)
{
    ifstream ifs(_filepath);
    if(!ifs)
    {
        cout << "配置文件出错" << endl;
        return;
    }
    getConfigMap();
}

//
//
//
//得到配置文件内容
void Configuration::getConfigMap()
{
    ifstream ifs(_filepath);
    if(!ifs.good())
    {
        cout<<"输出文件打开失败"<<endl;
    }
    else
    {
        string word,word1;
        while(ifs >> word)
        {
            ifs >>word1;
            _configMap.insert(map<string,string>::value_type (word,word1));
        }
        ifs.close();
    }
}  

//获得英文语料库路径
string Configuration::getenDictPath()
{
    return _configMap.find("enDictPath")->second;
}

//获得中文语料库路径
string Configuration::getcnDictPath()
{
    return _configMap.find("cnDictPath")->second;
}

//获得生成词典路径
string Configuration::getaimsdir()
{
    return _configMap.find("aimsdir")->second;
}

//获得索引文件路径
string Configuration::getindexdir()
{
    return _configMap.find("indexdir")->second;
}

//获得停用词文件路径
string Configuration::getstopdir()
{
    return _configMap.find("stopdir")->second;
}

//打印配置文件内容，测试用
void Configuration::printfconfigmap()
{
    cout << "打印一下" <<endl;
    map<string, string>::reverse_iterator iter;  
    for(iter = _configMap.rbegin(); iter != _configMap.rend(); iter++)  
        cout<<iter->first<<"----"<<iter->second<<endl;  
    string word={"SeverIP"};
    cout << _configMap.find(word)->second <<endl;
}

}//end of namespace yt
