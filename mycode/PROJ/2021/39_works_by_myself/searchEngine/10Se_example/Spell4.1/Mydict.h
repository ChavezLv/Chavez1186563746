#ifndef __MYDICT_H__
#define __MYDICT_H__

#include <pthread.h>

#include <vector>
#include <map>
#include <set>
#include <string>

using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

namespace wd{
class Mydict
{
public:
    static Mydict* getInstance(){
        pthread_once(&_once,init);
        return _mydict;
    }
    static void init(){
        if(!_mydict){
            _mydict=new Mydict();
        }
        ::atexit(destory);
    }
    static void destory(){
        if(_mydict){
            delete _mydict;
            _mydict=nullptr;
        }
    }
    void initIndexTable(const string& ,const string&);
    map<string,set<int>>& getIndexTable(){return _indexTable;}
    vector<pair<string,int>>& getDict(){return _dict;}
private:
    Mydict() {}
    ~Mydict() {}
     
    int nByte(const char&c);
    int length(const string&s);
private:
    static Mydict* _mydict;
    static pthread_once_t _once;
    vector<pair<string,int>> _dict;
    map<string,set<int>> _indexTable;
};

 }
#endif
