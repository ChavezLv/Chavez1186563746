#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Solution {
public:
    void adjustOddAndEven(int* nums, int numsSize){
        /* int i = 0; */
        int j = numsSize - 1;
        for(int i = 0; i < numsSize; ++i){
            if(i >= j){
                break;
            }
            while(j >= 0 && nums[j] % 2 == 0){
                --j;
            }
            if(nums[i] % 2 == 0){
                cout << nums[i] << endl;
                //even
                swap(nums[i], nums[j]);
                --j;
            }
        }                      
    }
    void swap(int &lNum, int &rNum){
        int tmp = lNum;
        lNum = rNum;
        rNum = tmp;
    }
};
int main()
{
    Solution sol;
    int n = 10;
    int nums[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sol.adjustOddAndEven(nums, n);
    for(int i = 0; i < n; ++i){
        cout << nums[i] << " ";
    }
    cout << endl;
    return 0;
}

