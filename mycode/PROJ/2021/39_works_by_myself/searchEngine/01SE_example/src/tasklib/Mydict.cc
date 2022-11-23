#include "../../libs/Mylog.hh"
#include "../../include/tasklib/Mydict.hh"
#include "../../include/tasklib/EditDistance.hh"
#include "../../include/locallib/Configuration.hh"

#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::pair;
using std::map;
using std::set;
using std::ifstream;

namespace wd
{

Mydict * Mydict::createInstance()
{
    if(_pInstance == nullptr) {
        _pInstance = new Mydict();
        Configuration conf;
        _pInstance->init(conf.getConfigMap()["enDictPath"].c_str(),
            conf.getConfigMap()["cnDictPath"].c_str());
        ::atexit(destroy);
    }
    return _pInstance;
}

static void split_dict(ifstream & ifs, vector<pair<string,int>> & dict)
{
    string line;
    while(getline(ifs, line)) {
        auto pos = line.find(" ");
        if(pos == 0 || pos == line.size() - 1) {
            continue;
        }

        size_t n = 0;
        while(pos != line.size() && line[pos + n] == ' ') {
            ++n;
        }

        dict.push_back(std::make_pair(line.substr(0, pos),
                ::atoi(line.substr(pos + n, line.size()).c_str())));
    }
}

void Mydict::init(const char * dictEnPath, const char * dictCnPath)
{
    ifstream ifs(dictEnPath);
    if(!ifs.good()) {
        logError("open dictEn");
        exit(1);
    }
    split_dict(ifs, _dict);

    ifs.close();
    ifs.open(dictCnPath);
    if(!ifs.good()) {
        logError("open dictCn");
        exit(1);
    }
    split_dict(ifs, _dict);

    size_t line = 0;
    for(auto k : _dict) {
        for(size_t idx = 0; idx != k.first.size(); ++idx) {
            int nBytes = nBytesCode(k.first[idx]);
            _index_table[k.first.substr(idx, nBytes)].insert(line);
            idx += nBytes - 1;
        }
        ++line;
    }
}

void Mydict::destroy()
{
    if(_pInstance != nullptr) {
        delete _pInstance;
        _pInstance = nullptr;
    }
}


vector<pair<string, int>> & Mydict::getDict()
{ return _dict; }

map<string, set<int>> & Mydict::getIndexTable()
{ return _index_table; }

Mydict * Mydict::_pInstance = nullptr; // Mydict::createInstance();


} // end of namespace wd
