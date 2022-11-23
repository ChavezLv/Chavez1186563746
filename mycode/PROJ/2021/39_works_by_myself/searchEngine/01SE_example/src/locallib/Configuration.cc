#include "../../libs/Mylog.hh"
#include "../../include/locallib/Configuration.hh"

#include <map>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

namespace wd
{

static const char * confKey[] = {
    "ServerIP",
    "Port",
    "ThreadpoolCap",
    "TaskQueSize",
    "cnDictPath",
    "enDictPath",
    "cnCorpusDir",
    "enCorpusDir",
    "stopCnPath",
    "stopEnPath",
    "cachePath",
    "UpdateCachesTimeInit",
    "UpdateCachesTimePeri",
    "maxResNum",

    "dict_path",
    "hmm_path",
    "user_dict_path",
    "idf_path",
};

Configuration::Configuration(const std::string & filepath)
    : _filepath(filepath)
{
    if(_cfgFile == nullptr) {
        _cfgFile = filepath.c_str();
    }

    ifstream ifs(_filepath);
    if(!ifs.good()) {
        logError("open confFile");
        exit(1);
    }

    string line;
    while(!ifs.eof()) {
        std::getline(ifs, line);
        if(line.length() > 0) {
            auto pos = line.find(" ");
            int n = 0;
            while(pos < line.size() && line[pos + n] == ' ') {
                ++n;
            }
            if(pos == std::string::npos) {
                logError("conf 格式不正确");
                continue;
            }
            _configMap[line.substr(0, pos)] = line.substr(pos + n, line.length());
        }
    }

    // 检查
    for(auto & k : confKey) {
        if(_configMap.find(k) == _configMap.end()) {
            logError("need ", k);
            exit(1);
        }
    }
}

const char * Configuration::_cfgFile = nullptr;

}// end of namespace wd

