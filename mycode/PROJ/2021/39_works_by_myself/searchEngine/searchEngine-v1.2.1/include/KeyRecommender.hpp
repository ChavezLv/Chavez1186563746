/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：KeyRecommender.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 14:55:48
*   @description：
*
================================================================*/


#ifndef __WD_KEYRECOMMANDER_HPP__
#define __WD_KEYRECOMMANDER_HPP__

#include "JsonMsg.hpp"
#include "TcpConnection.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <string>
#include <set>
using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using std::set;
using json = nlohmann::json;

namespace wd{
class Configuration;
using ConfigurationPtr = Configuration*;
/* class TcpConnection; */
/* using TcpConnectionPtr = TcpConnection*; */

class KeyRecommender{
public:
    KeyRecommender(const string &query, const TcpConnectionPtr &conn, ConfigurationPtr &config);
    /* KeyRecommander(const string &word, const TcpConnectionPtr &conn) */

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
    /* Configuration _config;//配置类对象 */
    ConfigurationPtr _config;//配置类对象
    vector<pair<string, int>> _zhDict;//中文词典
    unordered_map<string, set<int>> _zhDictIndex;//中文词典索引
    /* KeyRecommander(const string &word, const TcpConnectionPtr &conn) */
    /* : _msg(word) */
    /* , _conn(conn){ */

    /* } */

    /* void doQuery(){ */
    /*     std::cout << "KeyRecommande doQuery()" << std::endl; */
    /*     string response = JsonMsg(_msg.size(), 100, _msg).toJsonLayoutStream();//模拟回显服务 */
    /*     _conn->sendInLoop(response); */
    /* } */
/* private: */
    /* string _msg; */
    TcpConnectionPtr _conn;

};

}//end of namespace wd

#endif //__WD_KEYRECOMMENDER_HPP__
