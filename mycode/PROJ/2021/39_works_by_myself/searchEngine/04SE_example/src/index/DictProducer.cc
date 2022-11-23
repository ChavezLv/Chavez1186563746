#include "../../include/DictProducer.hpp"
#include "../../include/Configuration.hpp"
#include "../../include/EditDistance.hpp"

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <cctype>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::cerr;

namespace spellCorrect
{
DictProducer::DictProducer(const string& dir)
    :_dir(dir)
     ,_splitTool(nullptr)
{
    get_files();
}

DictProducer::DictProducer(const string& dir,SplitTool* splitTool)
    :_dir(dir)
     ,_splitTool(splitTool)
{
    get_files();
}

void DictProducer::build_dict()
{
    Configuration* config = Configuration::getInstance(_dir.c_str());
    map<string,string>& configMap = config->getConfigMap();
    for(auto& file:_engFiles)
    {
        ifstream ifs(file);
        if(!ifs)
        {
            cerr << "ifs is not good : " << file << endl;
            return;
        }
        string word;
        while(ifs)
        {
            ifs >> word;
            push_dict(word);
        }
        ifs.close();
    }
    //去掉停用词
    ifstream stop_ifs(configMap["stop_word_eng"]);
    if(!stop_ifs)
    {
        cerr << "ifs is not good : stop_words_eng.txt" << endl;
        return;
    }
    string word;
    while(stop_ifs >> word)
    {
        auto iter = _dict.find(word);
        if(iter != _dict.end())
        {
            _dict.erase(iter);
        }
    }
    stop_ifs.close();
}

void DictProducer::build_cn_dict()
{
    Configuration* config = Configuration::getInstance(_dir.c_str());
    map<string,string>& configMap = config->getConfigMap();
    for(auto& file:_zhFiles)
    {
        string context = ifsToString(file);
        vector<string> words = _splitTool->cut(context);
        for(auto& word: words)
        {
            push_cn_dict(word);
        }
    }
    ifstream stop_ifs(configMap["stop_word_zh"]);
    if(!stop_ifs)
    {
        cerr << "ifs is not good : stop_words_zh.txt" << endl;
        return;
    }
    string word;
    while(stop_ifs >> word)
    {
        auto iter = _dict.find(word);
        if(iter != _dict.end())
        {
            _dict.erase(iter);
        }
    }
    stop_ifs.close();
}

void DictProducer::store_dict()
{
    Configuration* config = Configuration::getInstance(_dir.c_str());
    map<string,string>& configMap = config->getConfigMap();
    ofstream ofs;
    ofs.open(configMap["dic"]);
    if(!ofs)
    {
        cerr << "ifs is not good : dict.dat" << endl;
        return;
    }
    for(auto& elem:_dict)
    {
        ofs << elem.first <<" " << elem.second << endl;
    }
    ofs.close();
}

void DictProducer::buildIndex()
{
    int i = 0;
    for(auto& elem : _dict)
    {
        string key = elem.first;
        for(int idx = 0; idx < key.size(); ++idx)
        {
            int nBytes = nBytesCode(key[idx]);
            string sub = key.substr(idx,nBytes);
            _index[sub].insert(i);
            idx += (nBytes -1);
        }
        ++i;
    }
}

void DictProducer::storeIndex()
{
    Configuration* config = Configuration::getInstance(_dir.c_str());
    map<string,string>& configMap = config->getConfigMap();
    ofstream ofs;
    ofs.open(configMap["idx"]);
    if(!ofs)
    {
        cerr << "ifs is not good : idx.dat" << endl;
        return;
    }
    for(auto& elem : _index)
    {
        ofs << elem.first << " ";
        for(auto& seq: elem.second)
        {
            ofs << seq << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

void DictProducer::show_files() const
{
    cout << ">> english files:" << endl;
    for(auto &file:_engFiles)
    {
        cout << file << endl;
    }
    cout << ">> chinese files:" << endl;
    for(auto &file:_zhFiles)
    {
        cout << file << endl;
    }
}

void DictProducer::show_dict() const
{
    for(auto &elem:_dict)
    {
        cout << elem.first << " : " << elem.second << endl;
    }
}

void DictProducer::get_files()
{
    Configuration* config = Configuration::getInstance(_dir.c_str());
    map<string,string>& configMap = config->getConfigMap();
    //英文语料
    string dir = getcwd(nullptr,0) + string("/") + configMap["englishbase"];
    cout << dir << endl;
    DIR* dirp = opendir(dir.c_str());
    if(nullptr == dirp)
    {
        perror("opendir");
        return;
    }
    struct dirent* pdirent;
    while((pdirent = readdir(dirp)) != nullptr)
    {
        cout << "type = " << static_cast<int>(pdirent->d_type) << endl;
        if(8 == pdirent->d_type)//普通文件
        {
            string s = dir + pdirent->d_name;
            _engFiles.push_back(s);
        }
    }
    closedir(dirp);

    //中文语料
    dir = getcwd(nullptr,0) + string("/") + configMap["chinesebase"];
    cout << dir << endl;
    dirp = opendir(dir.c_str());
    if(nullptr == dirp)
    {
        perror("opendir");
        return;
    }
    while((pdirent = readdir(dirp)) != nullptr)
    {
        cout << "type = " << static_cast<int>(pdirent->d_type) << endl;
        if(8 == pdirent->d_type)//普通文件
        {
            string s = dir + pdirent->d_name;
            _zhFiles.push_back(s);
        }
    }
    closedir(dirp);
}

void DictProducer::push_dict(const string& word)
{
    string newWord;
    for(auto& elem:word)
    {
        if(isalpha(elem))
        {
            if(elem < 'a')
            {
                newWord.push_back(elem + 32);
            }
            else
            {
                newWord.push_back(elem);
            }
        }
    }
    if(newWord.size() != 0)
    {
        ++_dict[newWord];
    }
}

void DictProducer::push_cn_dict(const string& word)
{
    string temp;
    for(int i = 0;i < word.size(); ++i)
    {
        int nBytes = nBytesCode(word[i]);
        if(1 == nBytes)
        {
            if(isalpha(word[i]))
            {
                if(word[i] < 'a')
                {
                    temp += (word[i] + 32);
                }
                else
                {
                    temp += word[i];
                }
            }
        }
        else
        {
            string sub = word.substr(i,nBytes);
            temp += sub;
            i += (nBytes - 1);
        }
    }
    ++_dict[temp];
}

string DictProducer::ifsToString(const string& filename)
{
    ifstream ifs(filename,std::ios::ate);
    if(!ifs)
    {
        cerr << "ifs is not good : " << filename << endl;
    }
    size_t length = ifs.tellg();

    char* buff = new char[length+1];
    ifs.seekg(0);
    ifs.read(buff,length);
    ifs.close();
    return string(buff);
}

}//end of namespace spellCorrect

using namespace spellCorrect;
void testDict()
{
    SplitToolCppJieba jieba;
    DictProducer producer("../conf/",&jieba);
    producer.show_files();
    producer.build_dict();
    producer.build_cn_dict();
    /* producer.show_dict(); */
    producer.store_dict();
    producer.buildIndex();
    producer.storeIndex();
    return;
}

#if 1
int main()
{
    testDict();
    return 0;
}
#endif

