/*
*  File    KeyWordRecommender.hpp
*  Author  WayneGreat
*  Date    2021-09-05  11:59:37
*  Describe 
*/

#ifndef __KEYWORDRECOMMENDER_HPP__
#define __KEYWORDRECOMMENDER_HPP__

#include <hiredis/hiredis.h>

#include <vector>
#include <string>
#include <unordered_map>

#include "Configuration.hpp"
#include "json.hpp"


using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using json = nlohmann::json;

namespace wd
{

class KeyRecommender
{
public:
    KeyRecommender(const string &query, Configuration &config);

    void doProcess();

    vector<pair<string, int>> &getZhDict()
    {
        return _zhDict;
    }

    unordered_map<string, set<int>> &getIndexTable()
    {
        return _zhDictIndex;
    }

private:
    void readInfoFromFile();
    void findWordInDict(vector<string> &);

    vector<string> doQuery(string &word);//执行查询,返回的是所有未可选未计算最小编辑距离的词汇
    int is_zh_ch(char p);//为sub函数所用判断字符是否是汉字
    string sub(string str, int start,int &flag ,int end = -1);
    //可以将中文,英文,中英文字符串逐单个提取出来

    //以下的所有函数是用来计算最小编辑距离
    string calSimilarity(vector<string> &);
    // vector<string> calSimilarity(vector<string> &);

    size_t nBytesCode(const char ch);
    size_t length(const string &str);
    int triple_min(const int &a, const int &b, const int &c);
    int editDistance(const string & lhs);//计算最小编辑距离-包括处理中英文

    void responseByJson(string);//使用json发送十个最小距离的单词给客户端

private:
    string _queryWord;//用户查询的词
    Configuration _config;//配置类对象
    vector<pair<string, int>> _zhDict;//中文词典
    unordered_map<string, set<int>> _zhDictIndex;//中文词典索引
};


}// end of namespace wd

#endif //KEYWORDRECOMMENDER_HPP
