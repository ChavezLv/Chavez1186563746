#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;
class Solution {
public:
    /*26. Remove Duplicates from Sorted Array */
    //删除多于1次的重复
    int removeDuplicate(vector<int>& nums){
        auto begin = nums.begin();
        auto end = nums.end();
        //unique :Forward iterator to the new end of the range
        return distance(begin,unique(begin,end));
    }
    /* int removeDuplicate(int nums[],int n){ */
    /*     return distance(nums,unique(nums,nums+n)); */
    /* } */

    /*80. Remove Duplicates from Sorted Array II*/
    //删除多于2次的重复
    //  test2: [0,0,1,1,1,1,2,3,3]  test1 : {1,1,1,2,2,3,} 
    int removeDuplicates(vector<int>& nums) {
        if(nums.size() <= 2){
            return nums.size();
        }
        int i = 1;
        for(vector<int>::iterator it = nums.begin() + 2; it != nums.end(); ++it){
            if((*it != *(it - 2) && nums[i] == nums[i-1])||(*it == *(it - 2) && nums[i] != nums[i-1])||(*it != *(it - 2)&& nums[i] != nums[i-1])){
                nums[++i] = *it;
            }
        }
        return i + 1;//i表示下标,返回总和
    }
};

/* class Solution { */
/* public: */
    int maxSubArray(vector<int>& nums) {
        int result = INT32_MIN;
        int count = 0;
        for (int i = 0; i < nums.size(); i++) {
            count += nums[i];
            if (count > result) { // 取区间累计的最⼤值（相当于不断确定最⼤⼦序终⽌位置）
                result = count;
            }
            if (count <= 0) count = 0; // 相当于重置最⼤⼦序起始位置，因为遇到负数⼀定是拉低总和
        }
        return result;
    }
/* }; */
int main()
{
    Solution sol;
    vector<int> nums = {1,1,1,2,2,3,};
    /* vector<int> nums  = {1 , 1 , 2,}; */
    /* int nums[]  = {1 , 1 , 2,}; */
    /* int ret = sol.removeDuplicate(nums,3); */
    /* int ret = sol.removeDuplicate(nums); */
    int ret = sol.removeDuplicates(nums);
    printf("ret:%d\n",ret);
    
    return 0;
}
