#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;
#include <unordered_map>
using std::unordered_map;

class LRUCache {
public:
    //以正整数作为容量 capacity 初始化 LRU 缓存
    LRUCache(int capacity)
    : _capacity(capacity)
    {
            
    }
        
    //如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1
    int get(int key) {
        auto it = _cache.find(key);
        if(it != _cache.end()){
            //找到的话, 更新一下list的位置
            //
            _t.splice(_t.begin(), _t, it->second);//挪到前面
            it->second = _t.begin();   
            
            return it->second->second;
        }
        return -1;
        
        //耗时更长
        /* size_t cnt = _cache.count(key); */
        /* if(cnt){ */
        /*     _t.splice(_t.begin(), _t, _cache[key]);//挪到前面 */
        /*     return _cache[key]->second; */
        /* } */
        /* return -1; */

        /* auto it1 = _cache.insert({key, list<std::pair<int,int>>().begin()}); */
        /* if(it1.second){ */
            
        /* } */
         
        /* auto it = _cache.insert({key, -1}); */
        /* if(it.second){ */
        /*     /1* _cache.erase(key); *1/ */
        /*     _cache.erase(_cache.begin()); */ 
        /*     return -1; */
        /* } */
        /* _t.remove(key); */
        /* _t.push_back(key); */
        /* return _cache[key]; */
    }
        
    //如果关键字已经存在，则变更其数据值；
    //如果关键字不存在, 则插入该组「关键字-值」。
    //当缓存容量达到上限时，它应该在写入新数据之前删除最久未使用的数据值，\
    //从而为新的数据值留出空间。
    void put(int key, int value) {
        /* auto it = _cache.insert({key, list<std::pair<int,int>>().begin()}); // 构造了一次变慢了*/
        /* if(it.second){ */
        /*     _t.push_front({key, value}); */ 
        /*     it.first->second = _t.begin(); */
        /*     if(_cache.size() > _capacity){ */
        /*         _cache.erase(_t.back().first); */
        /*         _t.pop_back(); */
        /*     } */
        /* }else{ */
        /*     it.first->second->second = value; */
        /*     _t.splice(_t.begin(), _t, it.first->second);// 用reverse() 错误 */
             
        /*     //it.first->second = _t.begin(); // 不需要 */
        /* } */

        /* auto it = _cache.insert({key, value});//使用find超时了 */
        /* if(it.second){ */
        /*     //没有找到该值, 插入成功, 插入list */
        /*     _t.push_back(key); */ 
        /*     if(_cache.size() > _capacity){ */
        /*         //超了, 删除一个, 说明是一个新的值 */
        /*         auto deleteKey = _t.begin(); */
        /*         _cache.erase(*deleteKey); */
        /*         _t.pop_front(); */
        /*     } */

        /* }else{ */
        /*     //如果插入失败, 说明找到, 则更新值 */
        /*     it.first->second = value; */
        /*     _t.remove(key); */
        /*     _t.push_back(key); */
        /* } */


        auto it = _cache.find(key);//使用find超时了
        if(it != _cache.end()){
            //如果找到,更新值, 同时更新list
            /* _t.remove(); 因为map存了list的位置, 直接用splice */
            
            _t.splice(_t.begin(), _t, it->second);// 把需要更新的pair挪到前面的新位置
            _t.begin()->second = value;
            /* _t.erase(it->second); */
            /* _t.push_front({key, value}); */
            /* _cache[key] = _t.begin(); */
            
            
        }else{
            //未找到, 先判断是否满了上限
            if(_cache.size() == _capacity){
                //满了, 先删除
                _cache.erase(_t.back().first);  
                _t.pop_back();
            }
            _t.push_front({key, value});//插入后面的
            _cache[key] = _t.begin();

        }

    }
    void print(){
        for(auto &elem : _cache){
            cout << "map: ";
            cout << elem.first << "line: " << elem.second->first << " "<< elem.second->second << " / ";
        }
        cout << endl;

        for(auto &elem : _t){
            cout << "list: ";
            cout << elem.first << " " << elem.second << " / ";
        }
        cout << endl;
    }
private:
    //注意设计
    size_t _capacity;
    unordered_map<int, list<std::pair<int, int>>::iterator> _cache;//缓存最大两个, hash更快
    /* list<int> _t;//这样每次遍历链表节点, 很耗时间 */
    list<std::pair<int, int>> _t;
    
    
};

void test01(){
    LRUCache* lr = new LRUCache(2);
    lr->put(1, 1);
    lr->print();

    lr->put(2, 2);
    lr->print();

    cout << "lr->get(1) : "<< lr->get(1) << endl;
    lr->print();

    lr->put(3, 3);
    lr->print();

    lr->put(2, 3);
    lr->print();

    cout << "lr->get(2) : "<< lr->get(2) << endl;
}
void test02(){
    LRUCache* lr = new LRUCache(2);
    lr->put(2, 1);
    lr->print();

    lr->put(1, 1);
    lr->print();

    /* cout << "lr->get(1) : "<< lr->get(1) << endl; */
    /* lr->print(); */

    lr->put(2, 3);
    lr->print();

    lr->put(4, 1);
    lr->print();

    cout << "lr->get(1) : "<< lr->get(1) << endl;
    cout << "lr->get(2) : "<< lr->get(2) << endl;
}
void test03(){
    struct timeval start, end;
    memset(&start, 0, sizeof(start));
    memset(&end, 0, sizeof(end));
    LRUCache* lr = new LRUCache(2);
    gettimeofday(&start, nullptr);
    for(int i = 1; i < 300000; ++i){
        lr->put(i, i);
        lr->get(i/2);
        lr->put(i/2, i/2);
        lr->get(i/2);
    }
    gettimeofday(&end, nullptr);
    cout << "time: " << (end.tv_sec - start.tv_sec) * pow(10, 6) \
                + end.tv_usec - start.tv_usec << " us"<< endl;

}
int main(int argc, char* argv[])
{

    test01();
    test03();
    return 0;
}

