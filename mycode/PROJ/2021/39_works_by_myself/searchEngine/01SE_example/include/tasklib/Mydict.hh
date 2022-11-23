/* <Mydict> -*- C++ -*- */
#ifndef __MYDICT_H__
#define __MYDICT_H__

#include <vector>
#include <map>
#include <set>

namespace wd
{

class Mydict
{
public:
    static Mydict * createInstance();
    void init(const char * dictEnPath, const char * dictCnPath);
    static void destroy();

    std::vector<std::pair<std::string, int>> & getDict();
    std::map<std::string, std::set<int>> & getIndexTable();

private:
    Mydict() {}

    std::vector<std::pair<std::string, int>> _dict; // 词典
    std::map<std::string, std::set<int>> _index_table; // 索引表

    static Mydict * _pInstance;
};

} // end of namespace wd
#endif

