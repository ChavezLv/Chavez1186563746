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


std::shared_ptr<string> WebPage::getSummary(const vector<string>& queryWords){
    std::vector<string> summaryVec;// = new std::vector<string>();
    istringstream iss(_docContent);
    string line;
    while(iss >> line){
        for(auto &word :queryWords){
            //npos可以表示string的结束位子，
            //是string::type_size 类型的，
            //也就是find（）返回的类型。find函数在找不到指定值得情况下会返回string::npos
            string::size_type n = line.find(word);
            if(n != string::npos){
                string limitAbstract;
                if(line.substr(0, n).size() > 10){

                    limitAbstract = line.substr(n - 10);//从n的前10开始
                }else{
                    limitAbstract = line.substr(n);//从n开始
                }

                if(limitAbstract.size() > 110){
                    summaryVec.push_back(line.substr(n, 110));
                }else{
                    summaryVec.push_back(limitAbstract);
                }
                break;
            } 

            if(summaryVec.size() >= 1) { break; }
        }//end of for(auto &word :queryWords){}
        if(summaryVec.size() >= 1) { break; }
    }//enf of while(iss >> line){}

    std::shared_ptr<string> summary(new string());
    for(auto &sentence: summaryVec){
        summary->append(sentence).append("......");
    }
    summary->append("\n");
/* cout << "WebPage::getSummary success" << endl; */
    correct_non_utf_8(summary.get());
    return summary;
}

int WebPage::correct_non_utf_8(string *str){
    int i,f_size=str->size();
    unsigned char c,c2,c3,c4;
    string to;
    to.reserve(f_size);
    //~ size_t pos = from->find("'advsearch': ' Avansert s");

    for(i=0 ; i<f_size ; i++){
        c=(unsigned char)(*str)[i];
        if(c<32){//control char
            if(c==9 || c==10 || c==13){//allow only \t \n \r
                to.append(1,c);
            }
        }else if(c<127){//normal ASCII
            to.append(1,c);
        }else if(c<160){//control char
            if(c2==128){//fix microsoft mess, add euro
                to.append(1,226);
                to.append(1,130);
                to.append(1,172);
            }
            if(c2==133){//fix IBM mess, add NEL = \n\r
                to.append(1,10);
                to.append(1,13);
            }
        }else if(c<192){//invalid for UTF8, converting ASCII
            to.append(1,(unsigned char)194);
            to.append(1,c);
        }else if(c<194){//invalid for UTF8, converting ASCII
            to.append(1,(unsigned char)195);
            to.append(1,c-64);
        }else if(c<224){//possibly 2byte UTF8
            c2=(unsigned char)(*str)[i+1];
            if(c2>127 && c2<192){//valid 2byte UTF8
                if(c==194 && c2<160){//control char, skipping
                    ;
                }else{
                    to.append(1,c);
                    to.append(1,c2);
                }
                i++;
            }else{//invalid UTF8, converting ASCII
                to.append(1,(unsigned char)195);
                to.append(1,c-64);
            }
        }else if(c<240){//possibly 3byte UTF8
            c2=(unsigned char)(*str)[i+1];
            c3=(unsigned char)(*str)[i+2];
            if(c2>127 && c2<192 && c3>127 && c3<192){//valid 3byte UTF8
                to.append(1,c);
                to.append(1,c2);
                to.append(1,c3);
                i+=2;
            }else{//invalid UTF8, converting ASCII
                to.append(1,(unsigned char)195);
                to.append(1,c-64);
            }
        }else if(c<245){//possibly 4byte UTF8
            c2=(unsigned char)(*str)[i+1];
            c3=(unsigned char)(*str)[i+2];
            c4=(unsigned char)(*str)[i+3];
            if(c2>127 && c2<192 && c3>127 && c3<192 && c4>127 && c4<192){//valid 4byte UTF8
                to.append(1,c);
                to.append(1,c2);
                to.append(1,c3);
                to.append(1,c4);
                i+=3;
            }else{//invalid UTF8, converting ASCII
                to.append(1,(unsigned char)195);
                to.append(1,c-64);
            }
        }else if(c<256){//invalid UTF8, converting ASCII
            to.append(1,(unsigned char)195);
            to.append(1,c-64);
        }else{
            cout<<"WTF? more than 256 values per Byte ? ("<<(unsigned int)c<<")"<<endl;
        }
    }
    *str=to;
    return 1;
}
    
string& WebPage::getTitle() {
    correct_non_utf_8(&_docTitle);
    return _docTitle;
}//{ return _docTitle; }

string& WebPage::getUrl(){
    correct_non_utf_8(&_docUrl);
    return _docUrl;
}// { return _docUrl; }

//按docId进行排序
/* bool operator< (const WebPage &lhs, const WebPage &rhs) */
/* { */
/*     if (lhs._docId < rhs._docId) { */
/*         return true; */
/*     } */
/*     else { */
/*         return false; */
/*     } */
/* } */

}// end of namespace wd
