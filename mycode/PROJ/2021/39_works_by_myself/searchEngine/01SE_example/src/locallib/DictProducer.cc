#include "../../libs/Mylog.hh"
#include "../../include/locallib/DictProducer.hh"
#include "../../include/locallib/Configuration.hh"
#include "../../include/locallib/SplitTool.hh"
#include "../../include/tasklib/EditDistance.hh"

#include <cstddef>
#include <dirent.h>
#include <limits.h>
#include <sstream>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::map;
using std::set;

namespace wd
{

inline bool is_alph(char c)
{
    return ('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >=c);
}

DictProducer::DictProducer(const std::string & dir)
    : _dir(dir)
{
    get_files();
}

DictProducer::DictProducer(const std::string & dir, SplitTool * splitTool)
    : _dir(dir)
    , _splitTool(splitTool)
{
    get_files();
}


void DictProducer::build_dict(const char * stopEnPath)
{
    string st, line;
    for(auto & file : _files) {

        ifstream ifs(file);
        if(!ifs.good()) {
            logError("open file");
            exit(1);
        }

        while(std::getline(ifs, line)) {
            for(char & ch : line) {
                if(is_alph(ch)) {
                    st += ch <= 'Z' ? ch + 32 : ch;
                } else if(st.size()) {
                    push_dict(st);
                    st.clear();
                }
            }
        }
    }

    // 去除屏蔽词
    if(stopEnPath) {
        ifstream ifs(stopEnPath);
        if(!ifs.good()) {
            logError("open stopEnPath");
            exit(1);
        }

        while(getline(ifs, line)) {
            // 当读取的文件是windows的txt文档时，每行结尾是\r\n
            // getline会去掉\n 但保留了\r
            if(line[line.size() - 1] == '\r') {
                _dict.erase(line.substr(0, line.size() - 1));
            } else {
                _dict.erase(line);
            }
        }
    }
}

void DictProducer::build_cn_dict(const char * stopCnPath)
{
    string st, line;
    std::stringstream buf;
    for(auto & file : _files) {

        ifstream ifs(file);
        if(!ifs.good()) {
            logError("open file");
            exit(1);
        }

        buf << ifs.rdbuf();
        buf << "\n";

        logInfo(file, " get");
    }

    vector<string> res;
    _splitTool->cut(buf.str()).swap(res);
    for(auto x : res) {
        push_dict(x);
    }

    logInfo("cut finish");

    // 去除屏蔽词
    if(stopCnPath) {
        ifstream ifs(stopCnPath);
        if(!ifs.good()) {
            logError("open stopCnPath");
            exit(1);
        }

        while(getline(ifs, line)) {
            if(line[line.size() - 1] == '\r') {
                _dict.erase(line.substr(0, line.size() - 1));
            } else {
                _dict.erase(line);
            }
        }
    }
}

void DictProducer::store_dict(const char * filepath)
{
    ofstream ofs(filepath);
    if(!ofs.good()) {
        logError("store_dict");
        exit(1);
    }

    auto it = _dict.cbegin();
    size_t line = 1;
    for(; it != _dict.cend(); ++it, ++line) {
        ofs << it->first << " " << it->second << "\n";
    }

    ofs.close();
}


void DictProducer::show_files() const
{
    for(auto & s : _files) {
        cout << s << endl;
    }
}

void DictProducer::show_dict() const
{
    for(auto & k : _dict) {
        cout << k.first << " " << k.second << endl;
    }
}


void DictProducer::get_files()
{
    DIR * dir;
    struct dirent * ptr;

    if((dir = opendir(_dir.c_str())) == nullptr) {
        logError("Open dir error!");
        exit(1);
    }
    logDebug("dir = ", _dir.c_str());

    char * abs_path = nullptr;
    if((abs_path = realpath(_dir.c_str(), nullptr)) == nullptr) {
        logError("realpath");
        exit(1);
    }

    while((ptr = readdir(dir)) != nullptr) {
        if(ptr->d_type == DT_REG) {
            _files.push_back(std::string(abs_path).append("/").append(ptr->d_name));
        }
    }
}

void DictProducer::push_dict(const std::string & word)
{
    auto it = _dict.find(word);
    if(it == _dict.end()) {
        _dict[word] = 1;
    } else {
        ++_dict[word];
    }
}

}// end of namespace wd
