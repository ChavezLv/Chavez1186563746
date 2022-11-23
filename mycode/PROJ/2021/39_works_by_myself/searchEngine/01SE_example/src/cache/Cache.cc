#include "../../libs/Mylog.hh"
#include "../../include/cache/Cache.hh"
#include <fstream>
#include <sstream>
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::istringstream;

namespace wd
{
Cache::Cache(size_t capacity)
    : _capacity(capacity)
    , _updateCount(0)
    , _updateStatus(false)
{}
Cache::~Cache()
{}
string Cache::getElement(const string & key)
{
    auto iter = _cacheMap.find(key);
    if(iter == _cacheMap.end()){
        return string();
    }
    else{
        _dataList.splice(_dataList.begin(), _dataList, iter->second);
        _cacheMap[key] = _dataList.begin();
        return _cacheMap[key]->second;
    }
}

void Cache::addElement(const string &key, const string & value)
{
    auto iter = _cacheMap.find(key);
    if(iter == _cacheMap.end()){
        /* 淘汰元素 */
        if(_dataList.size() == _capacity){
            _cacheMap.erase(_dataList.back().first);
            _dataList.pop_back();
        }
        /* 在链表头部插入新节点 */
        _dataList.push_front(make_pair(key, value));
        _cacheMap[key] = _dataList.begin();

        /* 如果有多个cache需要更新 */
        if(_updateStatus){
            ++_updateCount;
            _pendingUpdateData.push_front(make_pair(key,value));
        }
    }
    else{
        if(_cacheMap[key]->second != value){
            _cacheMap[key]->second = value;
        }
        _dataList.splice(_dataList.begin(),_dataList, iter->second);
        _cacheMap[key] = _dataList.begin();
    }
}

void Cache::readFromFile(const string & cachePath)
{
    ifstream ifscache(cachePath);
    if(!ifscache.good()){
        ofstream ofs(cachePath);
        if(!ofs.good()) {
            logError("open cache fail");
            return;
        }
        ofs.close();
        ifscache.open(cachePath);
    }
    string line,key,value;
    while(getline(ifscache,line)){
        istringstream iss(line);
        ifscache >> key >> value;
        if(key.size()) {
            addElement(key,value);
        }
    }
    ifscache.close();
    _updateStatus = true;
}
void Cache::writeToFile(string & cachePath)
{
    ofstream ofscache(cachePath,std::ios::out);
    if(!ofscache.good()){
        logError("write to cache fail");
        return ;
    }
    for(auto iter = _cacheMap.begin(); iter!= _cacheMap.end(); ++iter){
        string value = getElement(iter->first);
        ofscache << iter->first << " " << value << endl;
    }
    ofscache.close();
}
size_t Cache::getupdateCount() const
{
    return _updateCount;
}
void Cache::update(const Cache & cache)
{
    if(cache._pendingUpdateData.size() > 0){
        for(auto & node : cache._pendingUpdateData)
            addElement(node.first, node.second);
    }
}
void Cache::clearUpdateData()
{
    _updateCount = 0;
    _pendingUpdateData.clear();
}

}//end of namespace wd

