#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;

/*
 * 地上有一个m行n列的方格。一个机器人从坐标(0, 0)的格子开始移动，
 * 它每一次可以向左、右、上、下移动一格，但不能进入行坐标和列坐标
 * 的数位之和大于k的格子。例如，当k为18时，机器人能够进入方格(35, 37)，
 * 因为3+5+3+7=18。但它不能进入方格(35, 38)，因为3+5+3+8=19。
 * 请问该机器人能够到达多少个格子？
 * */

class Solution {
public:
    /* inline */
    int bitSum(int x){
        int bitS = 0;
        while(x){
            bitS += x % 10;
            x /= 10;
        }
        return bitS;
    }
    /* inline */
    int kSum(int x, int y){
        return bitSum(x) + bitSum(y);
    }
    int num(int x, int y, int row, int col, int k, int *arr[]){
        if(x < 0||y < 0||x >= col||y >= row||kSum(x, y) > k || arr[x][y] == 1){
            return 0;
        }
        arr[x][y] = 1;
        return num(x + 1, y, row, col, k, arr)\
            + num(x - 1, y, row, col, k, arr)\
            + num(x, y + 1, row, col, k, arr)\
            + num(x, y - 1, row, col, k, arr) + 1;
    }
    int count(int row, int col, int k){
        int **p = new int*[row];
        int cnt = 0;
        for(int i = 0; i < row; ++i){
            p[i] = new int[col]; 
            bzero(p[i], sizeof(int) * col);
        }

        cnt = num(0, 0, row, col, k, p);

        for(int i = 0; i < row; ++i){
            delete [] p[i];
        }
        delete [] p;
        
        return cnt;
    } 
};
int main()
{
    Solution sol;
    /* cout << sol.bitSum(123) << endl; */
    /* cout << sol.count(20, 20, 18) << endl;; */
    cout << sol.count(100, 100, 10) << endl;;
    /* cout << sol.count(100, 100, 20) << endl;; */
    /* cout << sol.count(5, 5, 3) << endl;; */
    cout << sol.count(11, 11, 10) << endl;;
    
    return 0;
}

