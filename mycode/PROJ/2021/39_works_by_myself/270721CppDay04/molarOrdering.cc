#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Solution {
public:
    static int molarOrdering(int arr[], int n){
        int cnt = 0;
        int num = 0;
        for(int i = 0; i < n; ++i){
            if(i == 0){
                num = arr[i];
                cnt = 1;
            }
            if(num == arr[i]){
                ++cnt;
            }
            if(num != arr[i]){
                --cnt;
                if(0 == cnt){
                    num = arr[i];
                    cnt = 1;
                }
            }
        }
        return num;
    } 
};
int main()
{
    int arr[] = {1, 2, 3, 2, 2, 2, 5, 4, 2};
    cout << Solution::molarOrdering(arr, 9) << endl;
    return 0;
}

