#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    vector<int>& strToNums(vector<string> & vec){
        for(auto &word : vec){
            int temp = 0;
            int bit = 1;
            if(isLegalNumber(word)){
                for(auto it = word.end() - 1; it >= word.begin(); --it){
                    if(*it != '+' && *it != '-'){
                        //a + b;
                        temp += ((*it) - '0') * bit;  
                        bit *= 10;
                    }
                }
                if(word[0] == '-'){
                    temp = 0 - temp;
                }
                _nums.push_back(temp);
            }
        } 
        return _nums;
    }

    bool isLegalNumber(const string & word){
        if(0 == word.size()){
            return false;
        }
        auto it = word.begin();
        if(*it == '+' || *it == '-'){
            ++ it;
        }
        for(; it != word.end(); ++it){
            if((*it) < '0' || (*it) > '9'){
                return false;
            }
        }
        return true;
    }
   
private:
    vector<int> _nums;
};
void test01(){
    vector<string> vec = {"abvc", "123", "+123", "-2343241", "-11234123412341234124121344232431"};
    vector<int> nums = Solution().strToNums(vec);
    for_each(nums.begin(), nums.end(), [](const int & elem){
                cout << elem << endl;
             });
    cout << endl;
}
int main(int argc, char* argv[])
{
    test01();
    return 0;
}
