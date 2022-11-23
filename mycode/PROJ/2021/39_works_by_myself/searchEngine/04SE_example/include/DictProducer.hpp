#ifndef __DICTPRODUCER_HPP__
#define __DICTPRODUCER_HPP__

#include "SplitToolCppJieba.hpp"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>

using std::string;
using std::vector;
using std::map;
using std::set;
using std::unordered_map;

namespace spellCorrect
{
class DictProducer
{
public:
    DictProducer(const string& dir);
    DictProducer(const string& dir,SplitTool* splitTool);

    void build_dict();
    void build_cn_dict();
    void store_dict();

    void buildIndex();
    void storeIndex();

    void show_files() const;//查看文件路径，测试用
    void show_dict() const;//查看词典，测试用
private:
    void get_files();//获取文件绝对路径
    void push_dict(const string& word);//存储某个单词
    void push_cn_dict(const string& word);//逐字符区分中英文存储词语
    string ifsToString(const string& filename);//根据文件名打开文件并存储为string

private:
    string _dir;//语料库文件夹相对地址//conf/
    vector<string> _engFiles;//语料文件绝对路径
    vector<string> _zhFiles;//语料文件绝对路径
    map<string,int> _dict;
    unordered_map<string,set<int>> _index;
    SplitTool* _splitTool;//中文分词工具//英文不需要分词工具，为nullptr，所用不用引用而是指针
};

}// end of namespace spell spellCorrect


#endif
