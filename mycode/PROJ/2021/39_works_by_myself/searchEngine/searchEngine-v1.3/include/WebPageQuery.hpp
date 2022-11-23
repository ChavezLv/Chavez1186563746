/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：WebPageQuery.hpp
*   @date       ：2021年09月05日 11:43:12
*   @description：
*
================================================================*/


#ifndef __WD_WEBPAGEQUERY_HPP__
#define __WD_WEBPAGEQUERY_HPP__

/* #include "Configuration.hpp" */
/* #include "SplitTool.hpp" */
#include "WebPage.hpp"
#include <math.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <set>
using std::unordered_map;
using std::set;
using std::pair;
using std::vector;
using std::string;
/* using std::unique_ptr; */
using std::shared_ptr;


namespace wd{

/* #define NEW_WEBPAGE_LIB_PATH_KEY "NEW_WEBPAGE_LIB_PATH" */ 
/* #define NEW_WEBPAGE_OFFSET_LIB_PATH_KEY "NEW_WEBPAGE_OFFSET_LIB_PATH" */
/* #define INVERT_INDEX_PATH_KEY "INVERT_INDEX_PATH" */
class WebPage;
class SplitTool;
using SplitToolPtr = SplitTool*;
class Configuration;
using ConfigurationPtr = Configuration*;

struct SimilariryCompare{
    vector<double> _base;
    SimilariryCompare(vector<double> &base)
    : _base(base){

    }

    bool operator()(const pair<int, vector<double> >&lhs, \
                    const pair<int, vector<double> >&rhs){
        double lhsDotProduct = 0;
        double rhsDotProduct = 0;
        double lhsQuadraticSum = 0;
        double rhsQuadraticSum = 0;

        for(size_t index = 0; index != _base.size(); ++index){
            lhsDotProduct += _base[index] * (lhs.second)[index];
            rhsDotProduct += _base[index] * (rhs.second)[index];
            lhsQuadraticSum += pow( (lhs.second)[index], 2 );
            rhsQuadraticSum += pow( (rhs.second)[index], 2 );
        }

        if(lhsDotProduct / sqrt( lhsQuadraticSum ) < rhsQuadraticSum / \
           sqrt(rhsQuadraticSum) ){
            return false;// lhs < rhs , 就要移动
        }else{
            return true;
        }
    }
    


};


class WebPageQuery{
public:
    WebPageQuery();

    string doQuery(const string &sought);
    string jsonLayOutNullResultString();
private:
    /* void loadDataFiles(); //done  in Configuration */
    void makeWebPageMember();
    
    vector<double> getQueryWordsWeighVector(vector<string> &queryWords);

    bool executeQuery(const vector<string> & queryWords, vector<pair<int, vector<double> > > &resultVec);

    /* string doResponse(); */


    string jsonLayOutString(const vector<int> & docIDVec, const vector<string>\
                            & queryWords);


private:
    SplitToolPtr _jieba; //单例类
    ConfigurationPtr _conf;// 单例类
    unordered_map<int, shared_ptr<WebPage> >& _pageLib; // 这个也可以放到Configure类里面, 就不用每次打开了
    unordered_map<int, pair<int, int> >& _offsetLib;
    unordered_map<string, set<pair<int, double> > > & _invertIndexTable; 
};

}//end of namespace wd

#endif //__WD_WEBPAGEQUERY_HPP__
