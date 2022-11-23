#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
using namespace std;
class Solution {
public:
    //排序后开始遍历+双指针
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ret;
        if (nums.size() < 3) {
            return ret;
        }
        sort(nums.begin(), nums.end());
        int sum = 0;
        int i, j;
        for (int k = 0; k < nums.size() - 2 && nums[k] <= 0; ++k) {
            i = k + 1;
            j = nums.size() - 1;
            //k>0  or  遇到相等需要去重
            if (k > 0 && nums[k] == nums[k - 1]) {
                continue;
            }
            while (i < j) {
                sum = nums[k] + nums[i] + nums[j];
                if (sum > 0 && j > 1) {
                    --j;
                }
                else if (sum < 0 && i < nums.size() - 1) {
                    //while (i<nums.size()-1&&nums[i] == nums[i + 1]) 
                    ++i;
                }
                else {
                    if (i > k + 1 && nums[i] == nums[i - 1]) {
                        ++i;
                        continue;
                    }
                    if (j < nums.size() - 1 && nums[j] == nums[j + 1]) {
                        ++j;
                        continue;
                    }
                    ret.push_back({ nums[k],nums[i],nums[j]  });
                    ++i;
                    --j;
                }
            }
        }
        return ret;
    }
};
int main()
{
    vector<int> nums = {-1, 0, 1, 2, -1, -4}; 
    Solution sol;
    vector<vector<int>> ret = sol.threeSum(nums);
    for(auto iter = ret.cbegin();iter!=ret.cend();++iter){
        /* for(auto it = (*iter).begin();it!=(*iter).end();++it){ */
        for(auto it = (*iter).cbegin();it!=(*iter).cend();++it){
            cout << (*it) << endl;
        }
    }
    return 0;
}

