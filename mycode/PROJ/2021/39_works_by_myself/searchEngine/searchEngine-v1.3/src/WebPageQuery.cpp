/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：WebPageQuery.cpp
*   @date       ：2021年09月05日 12:05:02
*   @description：
*
================================================================*/


#include "WebPageQuery.hpp"
#include "JsonMsg.hpp"
#include "Configuration.hpp"
#include "SplitTool.hpp"
#include "WebPage.hpp"
/* #include <fstream> */
/* #include <sstream> */
using std::stringstream;
using std::ifstream;
using std::make_pair;//!= cppjieba::make_pair

namespace wd{

WebPageQuery::WebPageQuery()
: _jieba(Singleton<SplitTool>::getInstance())
, _conf(Singleton<Configuration>::getInstance())// default path is ./conf/my.conf
, _pageLib(_conf->getPageLib())
, _offsetLib(_conf->getOffsetLib())
, _invertIndexTable(_conf->getInvertIndexTable()){
    /* loadDataFiles(); */
    /* makeWebPageMember(); */
}

string WebPageQuery::doQuery(const string &sought){
cout << " start WebPageQuery::doQuery(const string &sought)" << endl;
    vector<string> queryWords;
    if(sought.size()){
        queryWords = (*_jieba)(sought.c_str());
    } 
/* for(auto &word: queryWords){ */
/*     cout << word << " " ; */
/* } */
/* cout << endl; */
// cout << " WebPageQuery::doQuery(const string &sought) , queryWords()" << queryWords.size() << endl;
// cout << " WebPageQuery::doQuery(const string &sought) , invert" << _invertIndexTable.size() << endl;
//

    
for(auto &word : queryWords){
    cout << word << " ";
}
cout << endl;
    //为什么这里要用去停词， 因为同事的离线库没有使用去停词
    const set<string>& stopwords = _conf->getStopWordList();
    for(auto wordIt = queryWords.begin(); wordIt != queryWords.end(); ){
            // cout << "b erase" << endl;
        auto iter = _invertIndexTable.find(*wordIt);
            // cout << "a erase" << queryWords.size() << endl;
/* cout << "h" << iter->first << " "; */
        if(iter == _invertIndexTable.end() || (stopwords.find(*wordIt) != stopwords.end() && queryWords.size() > 1)){
            wordIt = queryWords.erase(wordIt);//没有找到关键字就从词集删除

        }else{
            ++wordIt;
        }
    }
// cout << " WebPageQuery::doQuery(const string &sought) , queryWords()" << queryWords.size() << endl;
    if(0 == queryWords.size()){
        return jsonLayOutNullResultString();//所有要查的词都没有就退出
    }
cout << endl;
for(auto &word : queryWords){
    cout << word << " ";
}
cout << endl;
    
    //计算每个搜索词的w
    vector<double> weightList = getQueryWordsWeighVector(queryWords);
/* for(auto &wi : weightList){ */
/*     cout << "wi" << wi << " "; */
/* } */
/* cout << endl; */
    //初始化比较SimilariryCompare的数据成员, 用于计算cosθ 
    SimilariryCompare similariryCmp(weightList);
// cout << " WebPageQuery::doQuery(const string &sought)" << endl;
    //start search
    vector<pair<int, vector<double> > > resultVec;
    if(executeQuery(queryWords, resultVec)){
/* cout << " start executeQuery WebPageQuery::doQuery(const string &sought)" << endl; */
        stable_sort(resultVec.begin(), resultVec.end(), similariryCmp); 

        vector<int> docIDVec;

        for(auto &pairType: resultVec){
            docIDVec.push_back(pairType.first);
        }
cout << "WebPageQuery:: docIDVec" << endl;
for(auto &id : docIDVec){
    cout << id << " " ;
}
cout << endl;

cout << " ret msg WebPageQuery::doQuery(const string &sought)" << endl;
        return jsonLayOutString(docIDVec, queryWords);

    }else{
cout << " null end WebPageQuery::doQuery(const string &sought)" << endl;
        return jsonLayOutNullResultString();
    }

    /* return ""; */
}

/* void WebPageQuery::makeWebPageMember(){ */
/*     vector<pair<int, string> > vecWebPage = _conf->getWebpages(); */ 

/*     for(auto &doc: vecWebPage){ */
/* cout << doc.first << ": --" << doc.second << endl; */
/*         WebPage webPage(doc.second, _conf, _jieba); */
/*         _pageLib.insert(make_pair(doc.first, webPage)); */
/*     } */
/* cout << "finish WebPageQuery::makeWebPageMember()" << _pageLib.size() << endl; */
/*    cout << _pageLib[0].getDocId() << endl; */ 
/* } */
    
vector<double> WebPageQuery::getQueryWordsWeighVector(vector<string> &queryWords){
/* cout << "start WebPageQuery::getQueryWordWeighVector" << endl; */
    map< string, int> wordFrequence;
    //统计word frequency
    for(auto &word: queryWords){
        ++wordFrequence[word];
    }

    vector<double> weightList;
    double weightSum = 0;
    int docSum = _pageLib.size();

    for(auto &word: queryWords){
        int df = _invertIndexTable[word].size();
        double idf = log(static_cast<double>(docSum) / df + 0.05) / log(2);

        int termFreq = wordFrequence[word];
        double w = idf * termFreq;

        weightSum += pow(w, 2);

        weightList.push_back(w);
    }

    for(auto &item: weightList){
        item /= sqrt(weightSum);
    }

/* cout << "end WebPageQuery::getQueryWordWeighVector" << endl; */
    return weightList;
}

bool WebPageQuery::executeQuery(const vector<string> & queryWords, vector<pair<int, vector<double> > > &resultVec){
    /* if(0 == queryWords.size()){  } */


/* cout << "WebPageQuery::executeQuery tf" << minIterSteps << " size : " << commonPairAndIterSteps.size() << endl; */

    //方法一  int double
    vector<pair<int, double> > pairs;
    for(auto &word : queryWords){
        pairs.insert(pairs.end(), _invertIndexTable[word].begin(), _invertIndexTable[word].end());
    }
    sort(pairs.begin(), pairs.end(), [](pair<int, double> lhs, pair<int, double> rhs){
            return lhs.first < rhs.first;
         });
cout << pairs.size() << endl;
    size_t resBin = 0, resEnd = 0;
    for(; resEnd < pairs.size(); ++ resEnd){
        if(pairs[resEnd].first == pairs[resBin].first && resEnd - resBin == queryWords.size() - 1){
            vector<double> w;//如果找到所有查询词都在文章中
            for(; resBin <= resEnd; ++resBin){
                w.push_back(pairs[resBin].second);
            }
            resultVec.push_back(make_pair(pairs[resEnd].first, w));
/* cout << "resultVec.size(): "<< resultVec.size() << endl; */
                        
        }
        if(pairs[resEnd].first != pairs[resBin].first && resBin < resEnd){
            resBin = resEnd;
        }

    }
    //结果集什么也没有, false
    if(resultVec.size() == 0){
        return false;
    }

    //方法二

    /* using pair_set_iterator = set<pair<int, double> >::iterator; */
    /* vector< pair< pair_set_iterator, int> > commonPairAndIterSteps; */

    /* int minIterSteps = 0x7fffffff;//means  min tf */

    /* for(auto &word: queryWords){ */
    /*     int tf = _invertIndexTable[word].size(); */
    /*     if(0 == tf){ */
    /*         return false; */
    /*     } */

    /*     if(minIterSteps > tf){ */
    /*         minIterSteps = tf; */
    /*     } */

    /*     commonPairAndIterSteps.push_back(\ */
    /*         make_pair( _invertIndexTable[word].begin(), 0)); */
    /* } */

    /* bool loopFlag = true; */

    /* while(loopFlag){ */
    /*     for(auto eachPairOfWord = commonPairAndIterSteps.begin(); eachPairOfWord == commonPairAndIterSteps.end(); ++eachPairOfWord){ */
            
    /*     } */
    /*     auto eachPairOfWord = commonPairAndIterSteps.begin(); */
    /*     for(;eachPairOfWord + 1 == commonPairAndIterSteps.end(); ++ eachPairOfWord){// eachPairOfWord.first = each Pair Of Word */
    /*         if(eachPairOfWord->first->first != \ */
    /*            (eachPairOfWord + 1)->first->first) { break; } */
    /*     } */

    /*     if(eachPairOfWord != commonPairAndIterSteps.end()){ */
    /*         //相同的docid */
    /*         vector<double> weightVec; */
    /*         int docId = commonPairAndIterSteps[0].first->first; */
    /*         for(size_t idx = 0; idx != commonPairAndIterSteps.size(); ++idx){ */
    /*             weightVec.push_back(commonPairAndIterSteps[idx].first->second); */
    /*             ++(commonPairAndIterSteps[idx].first); */
    /*             ++(commonPairAndIterSteps[idx].second); */

    /*             if(commonPairAndIterSteps[idx].second == minIterSteps) */
    /*             { loopFlag = false; } */

    /*         } */
    /*         resultVec.push_back(make_pair(docId, weightVec)); */
    /*     } */
    /*     else{ */
    /*         int minDocID = 0x7fffffff; */
    /*         int currentPairIndex; */
    /*         for(size_t idx = 0; idx != commonPairAndIterSteps.size(); ++idx){ */
    /*             if(commonPairAndIterSteps[idx].first->first < minDocID){ */
    /*                 minDocID = commonPairAndIterSteps[idx].first->first; */
    /*                 currentPairIndex = idx; */
    /*             } */
    /*         } */

    /*         ++(commonPairAndIterSteps[currentPairIndex].first); */
    /*         ++(commonPairAndIterSteps[currentPairIndex].second); */
    /*         if(commonPairAndIterSteps[currentPairIndex].second == minIterSteps){ */
    /*             loopFlag = true; */
    /*         } */
    /*     } */
    /* } */
    return true;
}

/* string WebPageQuery::doResponse(){ */

/* } */

string WebPageQuery::jsonLayOutString(const vector<int> & docIDVec, \
                                      const vector<string>& queryWords){
    int cnt = 0;
    json js =
    {
        {"msgLength", 0},
        {"msgType", 200},
        {"msgContent", {}}
    };
      
    for(auto &docId: docIDVec){
        string title = _pageLib[docId]->getTitle();
        string summary = *(_pageLib[docId]->getSummary(queryWords).get());
        string url = _pageLib[docId]->getUrl();
cout << "title: " << title << "summary: " << summary << "url:  " << url << endl;
        js["msgContent"].push_back(
                                   {
                                   {"title", title},
                                    {"summary", summary},
                                   {"url", url}
                                   }
                                  );
cout << " in for loop WebPageQuery:: js " << js << endl;

        if(++cnt == 10){
            break;
        }
    }
cout << "WebPageQuery:: js " << js << endl;
    string response = JsonMsg().toJsonLayoutStreamForWebSearch(js);
    cout << response << endl;
    return response;

    /* json js = */
    /* { */
    /*     {"msgLength", 0}, */
    /*     {"msgType", 200}, */
    /*     {"msgContent", {}} */
    /* }; */
/* cout << "WebPageQuery::jsonLayOutString" << endl; */
    /* for(auto &docId: docIDVec){ */
    /*     string title = _pageLib[docId]->getTitle(); */
    /*     string summary = _pageLib[docId]->getSummary(queryWords); */
    /*     string url = _pageLib[docId]->getUrl(); */
/* cout << "title: " << title << "summary: " << title << "url:  " << url << endl; */

    /*     js["msgContent"].push_back( */
    /*                                { */
    /*                                 {"title", title}, */
    /*                                 {"summary", summary}, */
    /*                                 {"url", url} */
    /*                                } */
    /*                               ); */

    /*     if(++cnt == 10){ */
    /*         break; */
    /*     } */
    /* } */
/* cout << "after for WebPageQuery::jsonLayOutString" << endl; */
    /* string response = JsonMsg().toJsonLayoutStreamForWebSearch(js);// ? */
/* cout << js << endl; */
    /* cout << response << endl; */
/* cout << "after cout  WebPageQuery::jsonLayOutString" << endl; */
    /* return response; */
}

string WebPageQuery::jsonLayOutNullResultString(){
    /* string title = "404 Not Found"; */
    /* string summary  = "Please try using a different keyword !"; */
    /* string url = "www.wangdao.com"; */
    string title = "404 Not Found";
    string summary  = "Please Try Using Different Keyword!";
    string url = "www.wangdao.com";
    json js =
    {
        {"msgLength", 1},
        {"msgType", 200},
        {"msgContent", {}}
    };
    js["msgContent"].push_back(
                               {
                                    {"title", title},
                                    {"summary", summary},
                                    {"url", url}
                               }
                              );

    /* string response = JsonMsg().toJsonLayoutStreamForWebSearch(js); */

    cout << "b to str WebPageQuery::jsonlayoutNULLResultstring" << endl << js << endl;
    string jsonLayout;
    jsonLayout = JsonMsg().toJsonLayoutStreamForWebSearch(js);
    /* std::stringstream ss; */
    /* ss << js; */
    /* ss >> jsonLayout; */
    cout << "a to str WebPageQuery::jsonlayoutNULLResultstring" << endl << jsonLayout<< endl;
    return jsonLayout;

}

/* void WebPageQuery::loadDataFiles(){ */
/*     ifstream ifsOffset(_conf->getConfigMap()[NEW_WEBPAGE_OFFSET_LIB_PATH_KEY], std::ios::in); */
/*     ifstream ifsWebpage(_conf->getConfigMap()[NEW_WEBPAGE_LIB_PATH_KEY]); */
/*     ifstream ifsInvert(_conf->getConfigMap()[INVERT_INDEX_PATH_KEY]); */
/*     if(!(ifsOffset.good() && ifsWebpage.good() && ifsInvert.good() ) ){ */
/*         cout << "WebPageQuery: data files error open!" << endl; */
/*         return; */
/*     } */

/*     int  docId, offset, length; */
/*     string line; */
/*     int cnt = 0; */
/*     while(getline(ifsOffset, line)){ */
/*         stringstream ss(line); */
/*         ss >> docId >> offset >> length; */

/*         _offsetLib.insert(make_pair(docId, make_pair(offset, length))); */

/*         string doc; */
/*         doc.resize(length, ' '); */
/*         ifsWebpage.seekg(offset, ifsWebpage.beg); */
/*         ifsWebpage.read(&*doc.begin(), length); */
/*         WebPage WebPage(doc, *_conf, *_jieba); */
/*         _pageLib.insert(make_pair(docId, WebPage)); */
/*     } */
/*     ifsOffset.close(); */
/*     ifsWebpage.close(); */

/*     string word; */
/*     double weight; */
/*     stringstream ss; */
/*     cnt = 0; */
/*     while(getline(ifsInvert, line)){ */
/*         ss.clear(); */
/*         ss << line; */
/*         set<pair<int, double> > setID; */
/*         //逐个读取 */
/*         ss >> word; */
/*         while(ss >> docId >> weight){ */
/*             setID.insert(make_pair(docId, weight)); */
/*         } */
/*         _invertIndexTable.insert(make_pair(word, setID)); */
        
/*     } */
/*     ifsInvert.close(); */

/*     cout << "WebPageQuery: data files ready!" << endl; */

/* } */

}//end of namespace wd

