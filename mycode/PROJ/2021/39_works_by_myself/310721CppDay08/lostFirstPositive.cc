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
    int firstMissingPositive(vector<int>& nums) 
    {
        int missNum = 1;
        /* for(auto &elem: nums){ */
        for(int i = 0; i < (int)nums.size(); ++i){
            if(nums[i] < 1 || nums[i] > (int)nums.size()){
                continue;
            }
            if(nums[i] == nums[nums[i] - 1]){
                continue;
            }
            while(nums[i] != i + 1 && nums[i] > 0 && nums[i] < (int)nums.size() + 1){
                if(nums[i] == nums[nums[i] - 1]){
                    break;
                }
                swap(nums[i], nums[nums[i] - 1]);
            }
        }

        int i = 0;
        for(i = 0; i < (int)nums.size(); ++i){
            if(nums[i] != i + 1){
                missNum = i + 1;
                return missNum;
            }
        }
        if(i == (int)nums.size()){
            missNum = i + 1;
        }
        return missNum;
    }
    void swap(int &lhs, int &rhs){
        int tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

};
int main()
{
    vector<int> nums = {4,3,4,1,1,4,1,4};
    cout << Solution().firstMissingPositive(nums) << endl;
    return 0;
}

