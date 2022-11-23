#include<string>
#include<iostream>
#include<fstream>
#include<regex>
#include<vector>
#include"tinyxml2.h"
using namespace tinyxml2;
using std::cout;
using std::endl;
using std::regex;
using std::string;
using std::vector;
struct RssItem {
    string title;
    string link;
    string description;
    string content;
};
   
class RssReader{
public:
#if 1
    RssReader();
    void parseRss(const char* &FileName);//解析
    void dump(const string & filename);//输出
#endif
private:
    vector<RssItem> _rss;
};  
