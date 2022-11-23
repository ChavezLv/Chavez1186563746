#ifndef __YT_GETINDEXFILE_HH__
#define __YT_GETINDEXFILE_HH__
#include "Configuration.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

class GetIndexFile
{
public:
    GetIndexFile(const string &aimsdir,const string &indexdir,const string &stopdir)
    :_aimsdir(aimsdir)
    ,_indexdir(indexdir)
    ,_stopdir(stopdir)
    {   
        getaimsdir();
        getstopdir();
    }

    ~GetIndexFile()
    {
    }
    
    //取出词典
    void getaimsdir();

    //取出停用词
    void getstopdir();

    //创建索引文件
    void create_index();

    //将索引文件写入文件
    void store_index();

    //判断是英文
    bool isEnglish(const string &rhs) const;


private:
    string _aimsdir;
    string _indexdir;
    string _stopdir;
    vector<pair<string,int>>_dict;
    unordered_set<string>_stopWords;
    unordered_map<string,set<int>>_index;
};
#endif
