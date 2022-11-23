#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
using namespace std;

/* template<typename _Tp> */
class LRU{
public:
    LRU(const int &capacity = 10)
    : _capacity(capacity){
        
    }
    /* using listIter = list<int>::iterator; */
    typedef list<int>::iterator listIter;

    int get(const int & key){
        if(_map.find(key) != _map.end()){
            //if exist , return value and update
            _list.splice(_list.begin(),  _list, _map[key]); 
            _map[key] = _list.begin();
            return *_map[key];
        }
        return -1;
    }

    void put(const int &key, const int &value){
        auto it = _map.find(key);
        if(it != _map.end()){
            //if exists
            _list.splice(_list.begin(), _list, it->second);
            _map[key] = _list.begin();
            *_map[key]  = value;
            
        }else{
            if(_capacity == _list.size()){
                int del = _list.back();
                _list.pop_back();
                auto it = _map.find(del);
                _map.erase(it);

            }
            _list.push_front(value);
            _map.insert(make_pair(key, _list.begin()));
        }
    }

private:
    size_t _capacity;
    unordered_map<int, listIter> _map;
    list<int> _list;
};
class Solution{
public:
    bool isPalindrome(const string &st){
        if(judgeStr(st, 0)){
            return true;
        }
        return false;
    }
    bool judgeStr(const string & st, int diffFlag){
        if(st.size() == 0){
            return false;
        }
        cout << st << endl;
        auto beg = st.begin();
        auto end = st.end() - 1;
        /* int diffFlag = 0; //只允许增加或者删除一个*/
        size_t steps = 1;
        size_t half = st.size() / 2;
        if(st.size() % 2 == 0){
            half = (st.size() - 1) / 2;
        }
        for(; beg != end && steps <= half;){
            if(*beg == *end){
                ++ beg;
                --end;
                ++ steps;
            }else{
                if(0 == diffFlag){
                    ++diffFlag;
                    /* string st(beg, end); */
                    cout << string(beg + 1, end - 1) << endl;
                    cout << string(beg + 1, end) << endl;
                    cout << string(beg, end - 1) << endl;
                    /* if(!judgeStr(string(beg + 1, end - 1)) || !judgeStr(string(beg + 1, end)) || \ */
                    /*    !judgeStr(string(beg, end - 1))){ */
                    if(!judgeStr(string(beg + 1, end), 1) || !judgeStr(string(beg + 1, end + 1), 1) || \
                       !judgeStr(string(beg, end), 1)){
                        return false;
                    }else{
                        return true;
                    }
                }else{
                    return false;
                }
            }
        }
        return true;
    }
/* private: */

};
void test01(){
    LRU lru;
    /* int index = 2; */
    for(int index = 0; index < 20; ++index ){
        lru.put(index, index);
        cout << lru.get(index) << endl;
    }
    for(int index = 0; index < 11; ++index){
        cout << lru.get(index) << endl;
    }
}

void test02(){
    cout << Solution().isPalindrome("") << endl;
    cout << Solution().isPalindrome("a") << endl;
    cout << Solution().isPalindrome("aba") << endl;
    cout << Solution().isPalindrome("abca") << endl;
    cout << Solution().isPalindrome("abaca") << endl;
    cout << Solution().isPalindrome("abaasdfdsaca") << endl;
}

int main(int argc, char* argv[])
{
    /* test01(); */
    test02();
    return 0;
}
