/*
*  File    WebPage.cc
*  Author  WayneGreat
*  Date    2021-09-02  21:06:36
*  Describe 
*/

#include "WebPage.hpp"
#include "Configuration.hpp"
#include "SplitTool.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

using std::cout;
using std::endl;
using std::vector;
using std::istringstream;

namespace wd
{

WebPage::WebPage(const string &doc, const ConfigurationPtr &conf, const SplitToolPtr &jieba)
// WebPage::WebPage(string &doc, Configuration &conf)
: _doc(doc)
{
    processDoc(doc, conf, jieba);
    // processDoc(doc, conf);
}

void WebPage::processDoc(const string &doc, const ConfigurationPtr &conf, const SplitToolPtr &jieba)
// void WebPage::processDoc(const string &doc, Configuration &conf)
{
    string docIdHead = "<docid>";
	string docIdTail = "</docid>";
	string docUrlHead = "<link>";
	string docUrlTail = "</link>";
	string docTitleHead = "<title>";
	string docTitleTail = "</title>";
	string docContentHead = "<content>";
	string docContentTail = "</content>";

    //提取docid
    int beg = doc.find(docIdHead);
    int end = doc.find(docIdTail);
    string docId = doc.substr(
        beg + docIdHead.size(),
        end - beg - docIdHead.size()
    );
    _docId = atoi(docId.c_str());

    //提取link
    beg = doc.find(docUrlHead);
    end = doc.find(docUrlTail);
    _docUrl = doc.substr(
        beg + docUrlHead.size(),
        end - beg - docUrlHead.size()
    );

    //提取title
    beg = doc.find(docTitleHead);
    end = doc.find(docTitleTail);
    _docTitle = doc.substr(
        beg + docTitleHead.size(),
        end - beg - docTitleHead.size()
    );

    //提取content
    beg = doc.find(docContentHead);
    end = doc.find(docContentTail);
    _docContent = doc.substr(
        beg + docContentHead.size(),
        end - beg - docContentHead.size()
    );

    // //将content进行分词，保存到vector中
    // cout << "Webpage.cc 68---" << endl;
    vector<string> wordVec = (*jieba)(_docContent.c_str());
    // for (auto &elem : wordVec) {
    //     cout << elem << " ";
    // }
    // cout << endl;

    //获取中文停用词
    set<string> &stopWordList = conf->getStopWordList();

    //统计文章中所有词语(除停用词后)的词频
    for (auto &elem : wordVec) {
        auto iter = stopWordList.find(elem);
        if (iter == stopWordList.end()) {//词语为非停用词
            ++_wordsMap[elem];
        }
    }
}


string WebPage::getSummary(const vector<string>& queryWords){
    std::vector<string> summaryVec;
    
    istringstream iss(_docContent);

    string line;
    while(iss >> line){
        for(auto &word :queryWords){
            //npos可以表示string的结束位子，
            //是string::type_size 类型的，
            //也就是find（）返回的类型。find函数在找不到指定值得情况下会返回string::npos
            if(line.find(word) != string::npos){
                summaryVec.push_back(line);
                break;
            } 

            if(summaryVec.size() >= 3) { break; }
        }//end of for(auto &word :queryWords){}

    }//enf of while(iss >> line){}

    string summary;
    for(auto &sentence: summaryVec){
        summary.append(sentence).append("\n");
    }
    return summary;
}
    
string WebPage::getTitle() {
    return _docTitle;
}//{ return _docTitle; }
string WebPage::getUrl(){
    return _docUrl;
}// { return _docUrl; }

//按docId进行排序
bool operator< (const WebPage &lhs, const WebPage &rhs)
{
    if (lhs._docId < rhs._docId) {
        return true;
    }
    else {
        return false;
    }
}

}// end of namespace wd
