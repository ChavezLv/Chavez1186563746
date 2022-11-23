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
    vector<vector<string>> solveNQueens(int n) {
        result.clear();//清零
        vector<string> vec(n, string(n, '.'));//初始化摆放方式
        backtracking(n, 0, vec);//从第0行开始
        return result;
    }

    void backtracking(int n, int row, vector<string>& vec) {
        if (row == n) {
            //退出条件, 递归n-1行已完成, 将结果放入result中
            result.push_back(vec);
            return;
        }
        for (int col = 0; col < n; col++) {
            if (isLegalPlace(row, col, vec, n)) { 
                vec[row][col] = 'Q'; //第一行能放的位置
                backtracking(n, row + 1, vec);//再看下一行可以放的位置, 直到退出条件
                vec[row][col] = '.'; // 退出，复原
            }
        }
    }
        
    bool isLegalPlace(int row, int col, vector<string>& vec, int n) {
        // 检查列
        for (int i = 0; i < row; i++) { // 剪枝
            if (vec[i][col] == 'Q') {
                return false;
            }
        }
        // 检查正斜线
        for (int i = row - 1, j = col - 1; i >=0 && j >= 0; i--, j--) {
            if (vec[i][j] == 'Q') {
                return false;
            }
        }
        // 检查反斜线
        for(int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
            if (vec[i][j] == 'Q') {
                return false;
            }
        }
        return true;
    }

private:
    vector<vector<string>> result;
};

void test01(){
    vector<vector<string>> vec = Solution().solveNQueens(1);
    for(auto &elem : vec){
        for(auto & it : elem ){
            cout << it << endl;
        }
        cout << endl;
    } 
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

