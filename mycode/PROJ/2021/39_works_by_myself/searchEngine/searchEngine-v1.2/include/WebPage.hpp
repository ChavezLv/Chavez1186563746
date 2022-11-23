/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：WebPage.hpp
*   @author     ：anonymous
*   @date       ：2021年09月05日 16:38:31
*   @description：
*
================================================================*/

#ifndef __WD_WEBPAGE_HPP__
#define __WD_WEBPAGE_HPP__

/* #include "Configuration.hpp" */

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

using std::map;
using std::set;
using std::sort;
using std::string;
using std::vector;


namespace wd{
class Configuration;
using ConfigurationPtr = Configuration*;
class SplitTool;
using SplitToolPtr = SplitTool*;


class WebPage
{
    // friend bool operator==(const WebPage&, const WebPage&);
    /* friend bool operator<(const WebPage&, const WebPage&); */
public:
    // const static int TOPK_NUMBER = 20;
    WebPage() {  }
    WebPage(const string &, const ConfigurationPtr&, const SplitToolPtr&);

    int getDocId()
    {
        return _docId;//获取文章id
    }

    map<string, int>& getWordsMap()
    {
        return _wordsMap;//获取文章{词，词频}
    }
    void setSimhash(uint64_t &u64)
    {
        _docSimhash = u64;
    }

    uint64_t getSimhash()
    {
        return _docSimhash;
    }

    string getDoc()
    {
        return _doc;//获取文章(含<标签>)
    }

    string getContent()
    {
        return _docContent;//获取文章内容
    }

    //for web search
    string getTitle() ;//{ return _docTitle; }
    string getSummary(const vector<string>& queryWords);
    string getUrl();// { return _docUrl; }


private:
    void processDoc(const string&, const ConfigurationPtr&, const SplitToolPtr&);

    // void CalTopK(vector<string>&, int, set<string>&);

private:
    string _doc;//<含标签>的文章
    int _docId;//文章id
    string _docTitle;//文章title
    string _docUrl;//文章url
    string _docContent;//文章内容
    string _docSummary;//摘要
    uint64_t _docSimhash;//文章simhash值
    // vector<string> _topWords;
    map<string, int> _wordsMap;//存储文章的{词，词频}
};

}//end of namespace wd

#endif //__WD_WEBPAGE_HPP__
