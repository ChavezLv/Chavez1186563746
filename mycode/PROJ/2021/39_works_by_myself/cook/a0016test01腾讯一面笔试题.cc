#include <iostream>

#include <string>

#include <vector>

#include <map>

#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> strToNum(vector<string> &vec){
        for(auto & elem: vec){
            if(isLeagel(elem)){
                int temp = 0; 
                int bit = 1;
                auto it = elem.end() - 1;
                for(; it >= elem.begin(); --it){
                    if((*it) >= '0' && (*it) <= '9'){
                        // intmax_t
                        if(!isMax(temp, bit * ((*it) - '0'))){
                            temp += bit * ((*it) - '0');
                            bit *= 10;
                        }else{
                            temp = 0x7fffffff;
                        }
                    }
                }
                if((*elem.begin()) == '-'){
                    temp = 0 - temp;
                    /* cout << temp << endl; */
                }
                /* cout << "true" << endl; */
                _vec.push_back(temp);
            }else{
                
                _vec.push_back(0);
            }
        }
        return _vec;
    }
    bool isMax(const int & lhs, const int &rhs){
        /* if(0x7fffffff - lhs < rhs || 0xffffffff - lhs > rhs){ */
        if(0x7fffffff - lhs < rhs){
            return true;
        }
        return false;
    }
    bool isLeagel(const string &lhs){
        if(0 == lhs.size()){
            return false;
        }
        for(auto &ch : lhs){
            if((ch < '0' || ch > '9' )\
               && ( (*lhs.begin()) != '-') && (*lhs.begin()) != '+'){ 
                return false;
            }
        }
        return true;
    }
private:
    vector<int> _vec;
    
};
void test01(){
    //  "abc " --> 0
    //
    /* vector<string> vec = {"abc", "cd1", "!asdf2", "1233"}; */       
    vector<string> vec = {"abc", "+456" , "cd1", "!asdf2", "-1233", "-1234567899999999999999999999", "99999999999999999999999999"};       
    /* vector<string> vec = {"-1233"}; */       
    vector<int> vecNum = Solution().strToNum(vec);
    for_each(vecNum.begin(), vecNum.end(), [](const int &num){
                cout <<  num << " " ;
             });
    cout << endl;
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}
