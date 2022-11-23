#ifndef __MYDICT_HPP__
#define __MYDICT_HPP__

#include <string>
#include <vector>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::set;

namespace spellCorrect
{

class MyDict
{
public:
    static MyDict* getInstance();

    void init(const char* dictPath,const char* indexPath);

    vector<pair<string,int>>& getDict();
    map<string,set<int>>& getIndex();

    struct AutoRelease
    {
    public:
        ~AutoRelease();
    };
private:
    MyDict() {}
    ~MyDict() {}

    void initDict(const char* dictPath);
    void initIndex(const char* indexPath);
private:
    vector<pair<string,int>> _dict;
    map<string,set<int>> _indexTable;

    static MyDict* _pInstance;
    static AutoRelease _auto;
};
}//end of namespace spellCorrect

#endif
