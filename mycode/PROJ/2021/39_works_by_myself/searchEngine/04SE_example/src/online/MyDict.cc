#include "../../include/MyDict.hpp"
#include "../../include/Configuration.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace spellCorrect
{
const string CONF_PATH = "../conf/";

MyDict* MyDict::_pInstance = nullptr;
MyDict::AutoRelease MyDict::_auto;

MyDict* MyDict::getInstance()
{
    if(!_pInstance)
    {
        Configuration* pcfg = Configuration::getInstance(CONF_PATH);
        auto conf = pcfg->getConfigMap();
        _pInstance = new MyDict;
        _pInstance->init(conf["dic"].c_str(),conf["idx"].c_str());
    }
    return _pInstance;
}

void MyDict::init(const char* dictPath,const char* indexPath)
{
    initDict(dictPath);
    initIndex(indexPath);
}

vector<pair<string,int>>& MyDict::getDict()
{
    return _dict;
}
map<string,set<int>>& MyDict::getIndex()
{
    return _indexTable;
}

MyDict::AutoRelease::~AutoRelease()
{
    if(MyDict::_pInstance)
    {
        delete MyDict::_pInstance;
        MyDict::_pInstance = nullptr;
    }
}

void MyDict::initDict(const char* dictPath)
{
    ifstream ifs(dictPath);
    if(!ifs)
    {
        cerr << "ifs_dictPath is not good" << endl;
        return;
    }
    pair<string,int> elem;
    while(ifs >> elem.first >> elem.second)
    {
        _dict.push_back(elem);
    }
    ifs.close();
}

void MyDict::initIndex(const char* indexPath)
{
    ifstream ifs(indexPath);
    if(!ifs)
    {
        cerr << "ifs_indexPath is not good" << endl;
        return;
    }
    string line;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        string word;
        iss >> word;
        auto& iter = _indexTable[word];
        int num;
        while(iss >> num)
        {
            iter.insert(num);
        }
    }
    ifs.close();
}

}//end of namespace spellCorrect

#if 0
using namespace spellCorrect;

void test()
{
    MyDict* pdict = MyDict::getInstance();
    auto dict = pdict->getDict();
    cout << "2" << endl;
    if(dict.empty())
    {
        cout << "4" << endl;
    }
    for(int i = 0 ;i < 10; ++i)
    {
        cout << dict[i].first << " : " << dict[i].second << endl;
    }

    auto index = pdict->getIndex();
    cout << "3" << endl;
    map<string,set<int>>::iterator iter = index.begin();
    for(int i = 0 ;i < 10; ++i)
    {
        cout << iter->first << " : ";
        for(auto elem:iter->second)
        {
            cout << elem << " ";
        }
        cout << endl;
        ++iter;
    }
}

int main()
{
    test();
}

#endif
