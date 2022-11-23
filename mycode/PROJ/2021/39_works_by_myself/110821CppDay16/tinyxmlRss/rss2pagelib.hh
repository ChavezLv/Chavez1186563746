#ifndef __RSS2PAGELIB_HH__
#define __RSS2PAGELIB_HH__

#include <iostream>
#include <vector>



using std::string;
using std::vector;

struct RssItem
{
    string title;//<title>  必需的。定义频道的标题。
    string link;//<link>    必需的。定义指向频道的超链接。
    string description;//<description>  必需的。描述频道。
    string content;
    
                
};

class RssReader
{
public:

    void parseRss(const string &filenameIn);//解析
    void dump(const string & filenameOut);//输出
private:
    vector<RssItem> _rssVec;//Once you have rssItem, put it into this vector  

};   

#endif
