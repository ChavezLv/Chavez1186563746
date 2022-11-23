#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Solution {
public:
    void sortColors(vector<int>& nums) {
        int i = 0;
        int j = nums.size() - 1;    
        for(int k = 0; k < (int)nums.size(); ++k){
            
            if((i == j && j == k) || k > j){
                break;
            }
            if(0 == nums[k]){
                if(i != k){
                    swap(nums[k], nums[i]);
                }
                ++i;
            }
            else if(1 == nums[k]){
                continue;
            }else{
                while(k < j && 2 == nums[k]){
                    swap(nums[k], nums[j]);
                    --j;
                }
                if(nums[k] == 0){
                    --k;
                }
            }
        }
    }
    void swap(int &lhs, int &rhs){
        int tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }
};
// {2, 0 , 1} error
// {2 } error
// {1 2 0} error
// {2, 2} error
int main()
{
    /* vector<int> nums = {2,0,2,1,1,0}; */
    vector<int> nums = { 2, 0}; // {2} error
    /* vector<int> nums = {2,0,1}; */
    Solution().sortColors(nums);
    for(auto &num : nums){
        cout << num << " ";
    }
    cout << endl;
    return 0;
}

