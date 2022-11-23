/* <DictProducer> -*- C++ -*- */
#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "Configuration.hh"
#include <string>
#include <vector>
#include <map>

namespace wd
{

class SplitTool;

class DictProducer
{
public:
    DictProducer(const std::string & dir);
    // 用于中文的处理
    DictProducer(const std::string & dir, SplitTool * splitTool);

    void build_dict(const char * stopWordsFile = nullptr);
    void build_cn_dict(const char * stopWordsFile = nullptr);
    // 写入文件
    void store_dict(const char * filepath);

    void show_files() const;
    void show_dict() const;

private:
    void get_files();
    void push_dict(const std::string & word);

private:
    std::string _dir; // 语料库的路径
    std::vector<std::string> _files;
    std::map<std::string, int> _dict;
    SplitTool * _splitTool;

};

} // end of namespace wd
#endif

