// 将 tinyxml2.h 和 tinyxml2.cc 放在同一文件夹当中，安装 boost 库，编译时加上 "-lboost_regex" 选项
// 部分截取输出和存储内容

#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string/regex.hpp>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

struct RssItem {
    string title;
    string link;
    string description;
    string content;
};

class RssReader {
public:
    void parseRss(const string& rss);
    void dumpData(const string& dat);

private:
    vector<RssItem> _rss;
};

void RssReader::parseRss(const string& rss) {
    XMLDocument doc;
    if (doc.LoadFile(rss.c_str())) {
        cout << "xml file not exist" << endl;
        return;
    }
    XMLElement* root = doc.RootElement();
    XMLElement* channel = root->FirstChildElement("channel");
    if (channel == nullptr) {
        cout << "root channel error" << endl;
        return;
    }
    XMLElement* item = channel->FirstChildElement("item");
    while (item) {
        RssItem node;
        cout << "fetch item success" << endl;
        XMLElement* subtitle = item->FirstChildElement();
        while (subtitle) {
            if (strcmp(subtitle->Name(), "title") == 0) {
                node.title.clear();
                node.title.append(subtitle->GetText());
                cout << "title------------------------------------------"
                    << endl;
            }
            if (strcmp(subtitle->Name(), "link") == 0) {
                node.link.clear();
                node.link.append(subtitle->GetText());
                cout << "link-------------------------------------------"
                    << endl;
            }
            if (strcmp(subtitle->Name(), "description") == 0) {
                node.description.clear();
                node.description.append(subtitle->GetText());
                cout << "description------------------------------------"
                    << endl;
            }
            if (strcmp(subtitle->Name(), "content:encoded") == 0) {
                node.content.clear();
                node.content.append(subtitle->GetText());
                cout << "content----------------------------------------"
                    << endl;
            }
            subtitle = subtitle->NextSiblingElement();
        }
        _rss.push_back(node);
        item = item->NextSiblingElement("item");
    }
}

void htmlReplace(string& content) {
    boost::erase_all_regex(content, boost::regex("<.+?>"));
}

void RssReader::dumpData(const string& dat) {
    ofstream ofs(dat);
    if (!ofs.good()) {
        return;
    }
    for (auto iter = _rss.begin(); iter != _rss.end(); iter++) {
        ofs << "<doc>" << endl;
        ofs << "<title>" << iter->title << "</title>" << endl;
        ofs << "<link>" << iter->link << "</link>" << endl;
        htmlReplace(iter->content);
        ofs << "<content>" << iter->content << "</content>" << endl;
        htmlReplace(iter->description);
        ofs << "<description>" << iter->description << "</description>" << endl;
        ofs << "</doc>" << endl;
    }
    ofs.close();
}

int main(int argc, char* argv[]) {
    RssReader rs;
    rs.parseRss("coolshell.xml");
    rs.dumpData("coolshell.dat");
    return 0;
}
