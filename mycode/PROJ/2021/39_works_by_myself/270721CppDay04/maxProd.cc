#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;

class Solution {
public:
    //非递归?
    int maxProd0(int n){
        int mid = n / 2;
        for(int i = 0; i < mid; ++i){

        }
        return 0;
    }
    int max = 0;
    int maxProd(int n){
        if(2 > n){
            return 0;
        }
        if(2 == n){
            return 1;
        }
        if(3 == n){
            return 2;
        }
        //record maxProd
        int* arr = new int[sizeof(int) * (n + 1)]();
        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 2;
        arr[3] = 3;
        /* int arr[] = {0, 1, 2, 3,}; */
        int max = 0;
        int tmpProd = 0;
        for(int i = 4; i <= n; ++i){
            max = 0;
            for(int j = 1; j <= i / 2; ++j){
                tmpProd = arr[j] * arr[i - j];
                max = max > tmpProd ? max : tmpProd;
            }
            arr[i] = max;
        }
        return arr[n];
    }

    int maxProdSplice(int n){
        int max = 0;
        int tmpProd = 0;
        for(int i = 0; i < n / 2; ++i){
            tmpProd = maxProd(i) * maxProd(n - i);
            max = max > tmpProd ? max : tmpProd;
        }
        return max;
    }

};
int main()
{
    int n;
    cin >> n;
    cout << Solution::maxProd(n) << endl;
    return 0;
}

