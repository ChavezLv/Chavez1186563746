#include "rss2pagelib.hh"
#include "tinyxml2.h"
#include <string>
#include <fstream>
#include <memory>
#include <regex>
using std::string;
using std::to_string;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::cerr;

using namespace tinyxml2;

void RssReader::parseRss(const string &filenameIn){ 
    // 1) LoadFile
    XMLDocument doc;    
    doc.LoadFile(filenameIn.c_str());
    if(doc.Error()){
        cout << "loading file(" << filenameIn << ") error." << endl; 
        return;
    }
    //doc.Print();
    
    // 2)
    /* XMLElement *root = doc.RootElement(); //"rss"*/ 

    //XMLNode::FirstChildElement( const char *name = 0  ) const f const XMLElement *
    /* auto *childElem= root->FirstChildElement(); //"channel" */
    /* auto *childElem= root->FirstChildElement()->FirstChildElement(); //"item"*/ 
    /* if(nullptr == childElem){ */
    /*     cout << "gets first child element failed" << endl; */
    /*     return; */
    /* } */
    /* cout << firstChildElem->GetText() << endl;//cout << 酷 壳 – CoolShell */
    /* cout << firstChildElem->Name() << endl;//title */

    auto *childElem = doc.FirstChildElement("rss")->FirstChildElement("channel")\
                           ->FirstChildElement("item");
    for(; childElem ;){
        RssItem items;

        auto *titleNode = childElem->FirstChildElement("title");
        items.title = titleNode->GetText();
        cout << items.title << endl;

        auto *linkNode = childElem->FirstChildElement("link");
        items.link = linkNode->GetText();
        cout << items.link << endl;

        std::regex reg("<[^>]+>");//去除所有 " <.... >"
        //比如:
        //from <p>本篇文章主要想讨论一下，Kubernetes 的 kubectl 命令中的使用到到的一个编程模式 &#8211; Visitor（注：其实，kubectl 主要...</p>
        //to 本篇文章主要想讨论一下，Kubernetes 的 kubectl 命令中的使用到到的一个编程模式 &#8211; Visitor（注：其实，kubectl 主要...
    
        auto *descriptionNode = childElem->FirstChildElement("description");
        items.description = regex_replace(descriptionNode->GetText(), reg, "");
        /* cout << items.description << endl; */

        auto *contentNode = childElem->FirstChildElement("content:encoded");
        items.title = regex_replace(contentNode->GetText(), reg, "");
        /* cout << items.content << endl; */

        _rssVec.push_back(items); 
        cout << _rssVec.size() << endl;

        //不需要以下遍历方式
        /* auto *itemChild = childElem->FirstChildElement(); */
        /* for(; itemChild; ){ */
        /*     if(itemChild->Name() == "title"){ */ 
        /*         items.title = "title"; */
        /*     } */        
        /*     itemChild = itemChild->FirstChildElement(); */
        /*     // .... */
        /* } */ 

        childElem = childElem->NextSiblingElement("item");
    }

}//解析: xml -->  rss --> tinyxml2 --> boost::regex/std::regex
// 1) RSS 指 Really Simple Syndication（真正简易联合）, RSS 使用 XML 编写
// 2) XML 指可扩展标记语言(EXtensible Markup Language) XML 是一种标记语言

void RssReader::dump(const string & filenameOut){
    if(0 == _rssVec.size()){
        cout << "null vector" << endl;
        return;
    }
    int cnt = 0;
    std::ofstream ofs(filenameOut);
    cout << filenameOut << endl;
    if(!ofs){
        cout << "sth. wrong happen" << endl;
        return;
    }
    //layout:
    /* <doc> */
    /*     <docid>1</docid> */
    /*     <title> ... </title> */
    /*     <link> ...  </link> */
    /*     <content> ... </content> */
    /* </doc> */
    for(auto &items : _rssVec){
        ofs << "<doc>" << endl;
        ofs << "\t<docid>" +  to_string(++cnt) + "</docid>" << endl; 
        cout << "\t<docid>" +  to_string(++cnt) + "</docid>" << endl; 
        ofs << "\t<title>" << items.title << "</title>" << endl; 
        ofs << "\t<link>" << items.link << "</link>" << endl;
        ofs << "\t<description>" << items.description << "</description>" << endl; 
        ofs << "\t<content>" << items.content << "</content>" << endl;
        ofs << "</doc>" << endl;
    }

    ofs.close();

}//输出


/*     int cnt = 1; */
/*     for(auto elem = _rss->begin(); elem != _rss->end(); ++elem) { */
/*         string wString; */
/*         wString = wString.append("<doc>\n\t") + */ 
/*             "<docid>" + std::to_string(cnt) + "</docid>\n\t" + */
/*             "<title>" + elem->title + "</title>\n\t" + */
/*             "<link>" + elem->link + "</link>\n\t" + */
/*             "<content>" + elem->content + "</content>\n" + */
/*             "</doc>\n"; */
/*         fwrite(wString.c_str(), 1, wString.size(), file); */
/*         ++cnt; */
/*     } */    

/*     fclose(file); */
/* } */
