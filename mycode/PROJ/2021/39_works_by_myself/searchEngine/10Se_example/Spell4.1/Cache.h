#ifndef __CACHE_H__
#define __CACHE_H__
#include "MutexLock.hpp"

#include <unordered_map>
#include <string>
#include <list>

using std::list;
using std::string;
using std::unordered_map;
namespace wd{

class Cache
{
public:
    Cache(int num=10);
    Cache(const Cache&);
    void addElement(const string&key,const string&value);
    string getElement(const string&key);
    void readFromFile(const string&filepath);
    void writeToFile(const string&filename);
    void update(const Cache&rhs);
    /* string find(const string&); */
    ~Cache() {}
    
    Cache& operator=(const Cache&rhs);
private:
    unordered_map<string,string> _hashmap;
    /* unordered_map<string,string> _dickhm; */
    list<string> _l;
    int _num;

    MutexLock _mutex;
};

}//end of namespace;
#endif
