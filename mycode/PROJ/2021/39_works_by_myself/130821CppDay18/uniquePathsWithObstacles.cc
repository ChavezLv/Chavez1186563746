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

class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();//初始化dp数组
        int n = obstacleGrid[0].size();        
        vector<vector<long>> dp(m, vector<long>(n, 1));//确定dp数组内容
        for(int i=0; i<m; ++i)
            for(int j=0; j<n; ++j)//确定遍历顺序
            {
                if(obstacleGrid[i][j] == 1)
                    dp[i][j] = 0;
                else if(i>0 && j>0)
                    dp[i][j] = dp[i-1][j] + dp[i][j-1];
                else if(i>0)
                    dp[i][j] = dp[i-1][j];
                else if(j>0)
                    dp[i][j] = dp[i][j-1];
                else
                    ;

            }
        return dp[m-1][n-1];

    }


};
void test01(){
    vector<vector<int>> obstacleGrid = { {0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    cout << Solution().uniquePathsWithObstacles(obstacleGrid) << endl;
}
void test02(){
    vector<vector<int>> obstacleGrid = { {0, 1}, { 0, 0}};
    cout << Solution().uniquePathsWithObstacles(obstacleGrid) << endl;
}
int main(int argc, char* argv[])
{

    test01();
    test02();
    return 0;
}

