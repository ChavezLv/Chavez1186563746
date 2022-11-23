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
//自己设置
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

    string Configuration::getenDictPath()
    {
        return _configMap.find("enDictPath")->second;
    }
    void getcnDictPath();
    string Configuration::getaimsdir()
    {
        return _configMap.find("aimsdir")->second;
    }

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
