#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    string maxCommon(vector<string> &vec){
        /* vector<char> merges; */
        /* for(auto &elem : vec){ */
        /*     merges.insert(merges.end(), elem.begin(), elem.end()); */
        /* } */
        /* sort(merges); */
        if(vec.size() == 1){
            return vec[0];
        }
        typedef string::iterator strIter;
        vector<strIter> strIt;
        size_t step = 0;
        size_t minStep = INT32_MAX;
        for(auto &elem : vec){
            strIt.push_back(elem.begin());
            if(elem.size() < minStep){
                minStep = elem.size();
            }
        } 

        bool loopFlag  = true;
        while(loopFlag){
            size_t index = 0;
            for(index = 0; index < strIt.size() - 1; ++ index){
                cout << "step char: "<< *strIt[index] << endl;
                if((*strIt[index]) != (*strIt[index + 1])){
                    break;
                }
            }

            if(index != strIt.size() - 1 && 0 == step){
                loopFlag = false;
            }else if(index == strIt.size() - 1){
                if(step == minStep){
                    return string(vec[0].begin(), vec[0].begin() + step);
                }
                for(auto &elem : strIt){
                    ++ elem;
                }
                ++step;
            }else if(index != strIt.size() - 1 && 0 != step){
                return string(vec[0].begin(), vec[0].begin() + step);
            }
        }
        return "";
    }

    void inverseInput(){
        string word;
        while(cin >> word){
            _stack.push(word);
        }
        while(!_stack.empty()){
            cout << _stack.top() << " ";
            _stack.pop();
        }
    }

private:
    stack<string> _stack;
};
void test01(){
    /* vector<string> vec = {"abc", "abd", "abef"}; */
    vector<string> vec = {"abasdfasdf", "abad", "abaef"};
    /* vector<string> vec = {"abc"}; */
    cout << Solution().maxCommon(vec) << endl;
}
void test02(){
    Solution().inverseInput();
}
int calGCD(const int & m, const int &n){
    int min = 0;
    min = m < n ? m : n;
    int temp = 0;
    for(int start = 1; start <= min; ++start){
        if(0 == m % start && 0 == n % start){
            temp = start;
        }
    }
    return temp;
}
int calLCM(const int & m, const int &n){
    int max = m > n ? m : n;
    int temp = 0;
    for(int start = m * n; start >= max; --start){
        if(0 == start % m && 0 == start % n){
            temp = start;
        }
    }
    return temp;
}
vector<int> calMax(const int & m, const int &n){
    return vector<int>({calGCD(m,n), calLCM(m, n)});    

}
int getRepeatedNum(vector<int> vec){
    /* int */ 
    /* for() */ 
    return 1;
}
void delNUM(string &lhs){
    for(auto it = lhs.begin(); it != lhs.end();){
        
        if((*it) >= '0' && (*it) <= '9'){
            it = lhs.erase(it);
        }else{
            ++it;
        }
    }
}
int main(int argc, char* argv[])
{

    /* test01(); */
    /* test02(); */

    /* int m = 0; */
    /* int n = 0; */
    /* cin >> m >> n; */
    /* vector<int> vec = calMax(m, n); */
    /* cout << "最大公约数和最小公倍数分别是: "; */
    /* for_each(vec.begin(), vec.end(), [](const int & elem){ */
    /*             cout << elem << " "; */
    /*          }); */
    /* cout << endl; */

    vector<int> vec = { 1, 2, 3, 4, 4 };
    cout << getRepeatedNum(vec) << endl;;
    return 0;
}
