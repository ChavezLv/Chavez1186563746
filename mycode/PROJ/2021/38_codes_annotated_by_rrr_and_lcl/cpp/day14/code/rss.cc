#include "tinyxml2.h"
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::regex_replace;
using std::vector;
using std::ofstream;

using namespace tinyxml2;

struct Record
{
    string title;
    string link;
    string description;
    string content;
};

class Rss
{
public:
    void parseRss();
    void dump(const string &filename);

private:
    vector<Record> _dict;
};
/* int test() */
void Rss::parseRss()
{
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.ErrorID())
    {
        cout << "open file error" << doc.ErrorID() << endl;
        return ;
    }

    XMLElement *itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    while(itemNode)
    /* if(itemNode) */
    {
        XMLElement *titleNode = itemNode->FirstChildElement("title");
        XMLElement *linkNode = itemNode->FirstChildElement("link");
        XMLElement *descriptionNode = itemNode->FirstChildElement("description");
        XMLElement *contentNode = itemNode->FirstChildElement("content:encoded");

        string title = titleNode->GetText();
        string link = linkNode->GetText();
        string description = descriptionNode->GetText();
        string content = contentNode->GetText();


        //regex reg("<[^>]+>|&nbsp|&amp?|&lt|&gt");
		regex reg("<[^>]+>");
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");

        cout << "title = " << title << endl;
        cout << "link = " << link << endl;
        cout << "description = " << description << endl;
        cout << "content = " << content << endl;

        _dict.push_back({title, link, description, content});

        itemNode = itemNode->NextSiblingElement();

    }
}

void Rss::dump(const string &filename)
{
    ofstream ofs(filename);
    if(!ofs)
    {
        std::cerr << "open " << filename << " error!" << endl;
        return;
    }

    for(size_t idx = 0; idx != _dict.size(); ++idx)
    {
        ofs << "<doc> " << endl
            << "<docid>" << idx + 1 << "</docid>" << endl
            << "<title>" << _dict[idx].title << "</title>" << endl
            << "<link>" << _dict[idx].link << "</link>" << endl
            << "<description>" << _dict[idx].description << "</description>" << endl
            << "<content>" << _dict[idx].content << "</content>" << endl
            << "</doc>"  << endl;
    }

    ofs.close();
}
int main(int argc, char **argv)
{
    Rss rss;
    rss.parseRss();
    rss.dump("wuhan.txt");
    return 0;
}

