#include "Cache.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#pragma comment(lib, "lib_json.lib")
#include "json/json.h"
using std::ifstream;
using std::ofstream;
using std::stringstream;
namespace wd{

Cache::Cache(int num)
    :_num(num)
{

}

Cache::Cache(const Cache& rhs){
    _hashmap=rhs._hashmap;
    _num=rhs._num;
    _l=rhs._l;
}

Cache& Cache::operator=(const Cache&rhs){
    if(this!=&rhs){
       _l=rhs._l;
       _hashmap=rhs._hashmap;
       _num=rhs._num;
    }
    return *this;
}
void Cache::addElement(const string&key,const string&value){
    auto iter=find(_l.begin(),_l.end(),key);//find
    if(iter==_l.end()){//如果在list里没有找到key
        if((int)_l.size()==_num){//判断是否容量达到了极限
            _hashmap.erase(_l.front());
            _l.pop_front();

        }
        _l.push_back(key);

    }
    else{//如果找到了key
        _l.splice(_l.end(),_l,iter);//改变list内元素的位置

    }
    _hashmap[key]=value;
}

string Cache::getElement(const string& key){
    if(_hashmap.find(key)!=_hashmap.end()){//如果存在key
        auto iter=find(_l.begin(),_l.end(),key);
        _l.splice(_l.end(),_l,iter);
        return _hashmap[key]; 

    }
    return "";
}

void Cache::readFromFile(const string&filepath){
    ifstream ifs(filepath);
    if(ifs.bad()){
        perror("open error!");
    }
    string word,jsonStr;
    getline(ifs,word);
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        perror("parse");
        return;
    }
    jsonStr=root.toStyledString();
    {
        MutexLockGuard prot(_mutex);
        addElement(word,jsonStr);
    }
    ifs.close();
}
void Cache::writeToFile(const string&filename){
    //先把磁盘的缓存以LRU的方式写到_hashmap中
    /* readFromFile(filename); */

    //然后再写入磁盘
    ofstream ofs(filename);
    for(auto &p:_hashmap){
        ofs<<p.first<<endl
           <<p.second<<endl;
    }
    ofs.close();
}

void Cache::update(const Cache& rhs){
    MutexLockGuard prot(_mutex);
    for(auto &p:rhs._hashmap){
        addElement(p.first,p.second);
    }
}

/* string Cache::find(const string& word){ */
/*     if(_hashmap.find(word)!=_hashmap.end()){ */
/*         return _hashmap[word]; */
/*     } */
/*     return ""; */
/* } */
}//end of namespace;
