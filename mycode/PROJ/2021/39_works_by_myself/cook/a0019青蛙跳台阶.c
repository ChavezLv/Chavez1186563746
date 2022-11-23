/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0019青蛙跳台阶.c
*   @author     ：anonymous
*   @date       ：2021年09月25日 10:33:38
*   @description：
*
================================================================*/


#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Solution{
public:
    Solution()
    : _jumps(0){

    }

    //用回溯递归去枚举就是会超时, 而且还要用额外的空间去存
    int getJumps(const int &n){
        /* if(0 == n){ */
        /*     return 1; */
        /* } */
        //思路: leetcode上面就是这种情形
        /* vector<int> vec = {1, 2};//f(n) = f(n-1) + f(n-2) */
        /* vector<int> vec = {1, 2, 3};//f(n) = f(n-1) + f(n-2) + f(n-3) */
        vector<int> vec = {1, 2, 3, 4};//f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4)
        backTacking(vec, 0, n);
        /* sort(_results.begin(), _results.end()); */
        /* _results.unique(); */
        /* return _results.size(); */
        return _jumps;
    }
    void backTacking(vector<int> vec, int sum, int n){
        if(sum == n){
            /* _results.push_back(std::move(_result));//error , 段错误*/
            ++_jumps;
        }

        for(int start = 0; start < (int)vec.size(); ++ start){
            if(sum + vec[start] > n){
                continue;
            }
            _result.push_back(vec[start]);
            sum += vec[start];
            backTacking(vec, sum, n);
            _result.pop_back();
            sum -= vec[start];
        }
    }

    int numWays(int n){
        _dp = {1, 1, 2};
        /* if(n < 3){ */
        /*     return _dp[n]; */
        /* } */
        int way = 0;
        for(int start = 3; start <= n; ++ start){
            way = _dp[start - 1] + _dp[start - 2];
            if(way > 1000000007){
                way = (_dp[start - 1] + _dp[start - 2]) % 1000000007;
            }
            _dp.push_back(way);
        } 
        return _dp[n];
    }
private:
    vector<vector<int>> _results;//不需要
    /* unique<vector<int>> _result; */
    vector<int> _result;//不需要
    int _jumps;
    vector<int> _dp;
};

void test01(){
    cout << Solution().getJumps(0) << endl;
    cout << Solution().getJumps(1) << endl;
    cout << Solution().getJumps(2) << endl;
    cout << Solution().getJumps(3) << endl;
    cout << Solution().getJumps(4) << endl;
    cout << Solution().getJumps(5) << endl;
    cout << Solution().getJumps(6) << endl;
    cout << Solution().getJumps(7) << endl;
    cout << Solution().numWays(52) << endl;
    cout << Solution().numWays(152) << endl;
}

int main(){
    test01();
    return 0;
}
