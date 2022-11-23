#include "tinyxml2.h"
#include <iostream>
#include <string>
#include <regex>

using std::cout;
using std::endl;
using std::string;

using namespace tinyxml2;

void test()
{
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.ErrorID())
    {
        std::cerr << "loadFile fail" << endl;
        return;
    }

    XMLElement *itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    if(itemNode)
    {
        string title = itemNode->FirstChildElement("title")->GetText();
        string link=  itemNode->FirstChildElement("link")->GetText();
        string description = itemNode->FirstChildElement("description")->GetText();
        string content = itemNode->FirstChildElement("content:encoded")->GetText();

        std::regex reg("<[^>]+>");
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");
    
        cout << "title = " << title << endl;
        cout << "link = " << link << endl;
        cout << "description = " << description << endl;
        cout << "content = " << content << endl;
    }
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

