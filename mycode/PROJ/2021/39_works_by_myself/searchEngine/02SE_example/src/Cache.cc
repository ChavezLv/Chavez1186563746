#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/Cache.hpp"

namespace wd
{
using std::endl;
using std::ofstream;
using std::pair;
using std::ifstream;
using std::string;
using std::istringstream;
Cache::Cache(size_t num):_maxCache(num)
{}
Cache::Cache(const Cache& cache):_maxCache(cache._maxCache)
{
    update(cache);
}
Cache &Cache::operator=(const Cache& cache)
{
    if(this==&cache)return *this;
    update(cache);
    return *this;
}

void Cache::addElement(const std::string &key,const std::string &val)//往缓存中添加数
{
    put(key,val);
}
void Cache::readFromFile(const std::string &filename)//从文件中读取缓存信息
{
    string data;
    ifstream ifs(filename);
    while(getline(ifs,data))
    {
        istringstream iss(data);
        pair<string,string> _pair;
        iss>>_pair.first>>_pair.second;
        put(_pair.first,_pair.second);
    }
    ifs.close();
}
void Cache::writeToFile(const std::string &filename)//将缓存信息写入文件
{
    ofstream ofs(filename);
    for(auto it=_hashtable.begin();it!=_hashtable.end();++it)     
    {
        ofs<< it->second->first <<" "<< it->second->second<<endl;
    }
    ofs.close();
}

void Cache::update(const Cache &cache)
{
    /*因为链表头放新数据，链表因为反向同步，新数据最后插入*/
   for(auto it=cache._list.rbegin();it!=cache._list.rend();++it)
   {
        put(it->first,it->second);
   }

}

/* 查询代词是否在缓存中有了结果，就不必继续后面的查询 
   如果找到了，就把词提升到最开头
 */
string Cache::hashFind(const std::string &word)
{
    string s=get(word);    
    return s;
}

string Cache::get(const string &key)
{
    auto it=_hashtable.find(key);
    if(it==_hashtable.end())//没找到
    {
        return string("");
    }
    _list.splice(_list.begin(),_list,it->second);//splice不会造成迭代器失效，不用操作map
    return it->second->second;
}
void Cache::put(const string &key,const string &val)
{
    auto it=_hashtable.find(key);
    if(it!=_hashtable.end())
    {
        _list.push_front(make_pair(key,val));
        _list.erase(it->second);
        _hashtable.erase(it);
        _hashtable[key]=_list.begin();
    }
    else
    {
        if(_hashtable.size()>=_maxCache)
        {
            _hashtable.erase(_list.back().first);
            _list.pop_back();
        }
        _list.push_front(make_pair(key,val));
        _hashtable[key]=_list.begin();
    }
}
}//end
