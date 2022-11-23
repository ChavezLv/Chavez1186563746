#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::vector;
using std::cout;
using std::string;
using std::endl;
/*
 *题目：请实现一个函数用来匹配包含'.'和'*'的正则表达式。模式中的字符'.'
*表示任意一个字符，而'*'表示它前面的字符可以出现任意次（含0次）。
*在本题中，匹配是指字符串的所有字符匹配整个模式。
*例如，字符串"aaa"与模式"a.a"和"ab*ac*a"匹配，但与"aa.a"及"ab*a"均不匹配。
 * */
class Solution {
public:
    bool judge(string str1, string str2){
        //aaa   ab*ac*a
        vector<char> vc1;
        vector<char> vc2;
        vc1.assign(str1.begin(), str1.end());
        vc2.assign(str2.begin(), str2.end());
        
        auto it2 = vc2.cbegin();
        for(auto it = vc1.cbegin(); it != vc1.cend(); ++it){
            if(it2 == vc2.cend()){
                return false;
            }
            /* if(it == vc1.cbegin() + 1 && *(it2 - 1) == '*'){ */
            /*     //暂时不考虑了a  b*b*a */
            /*     --it; */
            /* } */
            if(*it == *it2|| *it2 == '.'){
                ++it2;
                continue;
            }else{
                /* if(it == vc1.cbegin() && (it2 + 1) != vc2.cend() && *(it2 + 1) != '*'){ */
                /*     return false; */
                /* } */
                if(it2 != vc2.cbegin() && *it2 == '*' && *(it2-1) == *it){
                    //bbbbbba b*
                    continue;
                }
                if(it != vc1.cbegin() && (it2 + 1) != vc2.cend() && *(it2 + 1) == '*') {
                    /* cout << *it << endl; */
                    /* cout << *it2 << endl; */
                    //a b*a
                    ++it2;
                    ++it2;
                    /* if(it != vc1.cbegin()){ */
                        --it;//回退
                    /* } */
                    continue;
                }
            }
        }
        if(it2 != vc2.end() && *it2 != '*'){
            return false;
        }
        return true;
    } 
};
int main()
{
    Solution sol;
    cout << sol.judge("aaa", "a*a") << endl;
    cout << sol.judge("aaa", "a.a") << endl;
    cout << sol.judge("aaa", "ab*ac*a") << endl;
    cout << sol.judge("aaa", "aa.a") << endl;
    cout << sol.judge("aaa", "ab*a") << endl;
    return 0;
}

