#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip> 
using namespace std;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::setiosflags;
using std::setw;
/* using std::ios_base::setf; */
class Solution {
public:

};
void test(){
    /* cout.ios_base::setf(std::left); */
    /* cout.setf(std::left) <<setw(7); */
    cout.width(7);
    cout << "1" << endl;
    cout<< setiosflags(ios::left) << setw(7);
    cout << "1" << endl;

}
void test1(){
    map<string, set<int> > _word2Numbers;    
    set<int> num{1};
    _word2Numbers.insert(pair<string, set<int>>("abc", num));
    /* _word2Numbers.insert(pair<string, set<int>>("abc", num)); */
    set<int> num1{2};
    cout << *_word2Numbers["abc"].begin() << endl;
    num1.insert(_word2Numbers["abc"].begin(),_word2Numbers["abc"].end());
    _word2Numbers.insert(pair<string, set<int>>("abc", num1));
    set<int> num2{3};
    /* _word2Numbers.insert(pair<string, set<int>>("asdfasdf", num2)); */
    _word2Numbers.insert({"abd", num2});
    for(auto &elem: _word2Numbers){
        /* cout << elem.first << endl; */
        for(auto &elem1 : elem.second){
            cout << elem.first << ": " << elem1 << endl;
        }
    } 
     
}
int main()
{
    /* int a= 1; */
    /* int*p = &a; */
    /* const int &q = p; */
    test1();
    map<string ,int > nums{{"a", 1}, {"b", 1}, {"king", 1}};

    auto ret = nums.insert({"a", 1});
    if(ret.second){
        cout << ret.first->first << endl;
    }else{
        ++ nums["a"];
    }

    cout << nums["a"] << endl;
    return 0;
}

