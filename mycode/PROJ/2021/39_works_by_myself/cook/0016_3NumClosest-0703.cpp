#include <iostream>
#include <vector>
#include <algorithm>

/*
 * 题目大意 #
 * 给定一个数组，要求在这个数组中找出 3 个数之和离 target 最近。
 *
 * 解题思路 #
 * 这一题看似和第 15 题和第 18 题很像，都是求 3 或者 4 个数之和的问题，但是这一题的做法和 15，18 题完全不同。
 *
 * 这一题的解法是用两个指针夹逼的方法。先对数组进行排序，i 从头开始往后面扫。这里同样需要注意数组中存在多个重复数字的问题。具体处理方法很多，可以用 map 计数去重。这里笔者简单的处理，i 在循环的时候和前一个数进行比较，如果相等，i 继续往后移，直到移到下一个和前一个数字不同的位置。j，k 两个指针开始一前一后夹逼。j 为 i 的下一个数字，k 为数组最后一个数字，由于经过排序，所以 k 的数字最大。j 往后移动，k 往前移动，逐渐夹逼出最接近 target 的值。
 *
 * */

using namespace std;

class Solution {
public:
    int threeSumClosest(vector<int>&nums, int target) {
        int  retSum = 0;
        if (nums.size() < 3) {
            return retSum;

        }
        int tmpSum = 0;
        sort(nums.begin(),nums.end());
        retSum = nums[0] + nums[1] + nums[nums.size() - 1];//第一次循环时候tmp
        for (auto it = nums.cbegin(); it < nums.cend() - 2; ++it) {
            if (it > nums.cbegin() && *it == *(it - 1)) {
                continue;

            }
            auto i = it + 1;
            auto j = nums.cend() - 1;
            while (i < j) {
                tmpSum = *it + *i + *j;
                if (tmpSum == target) {
                    return tmpSum;

                }
                if (target >= 0 && *it > target && abs(retSum - target) < abs(tmpSum - target)) {
                    return retSum;

                }
                if (abs(retSum - target) > abs(tmpSum - target)) {
                    retSum = tmpSum;
                    //然后去重
                    /*while (i < j && *i == *(i + 1)) {
                      ++i;
                      }
                      while (i < j && *j == *(j - 1)) {
                      ++j;
                      }*/
                }
                if (tmpSum > target) {
                    --j;
                    if (abs(retSum - target) < abs(tmpSum - target)) {
                        continue;
                    }
                }
                if (tmpSum < target) {                             
                    ++i;
                    if (abs(retSum - target) < abs(tmpSum - target)) {
                        continue;
                    }
                }
                /*     if (target < 0 && *it < target && abs(retSum - target) < abs(tmpSum - target)) {
                       return retSum;
                       }*/
                if (abs(retSum - target) > abs(tmpSum - target)) {
                    retSum = tmpSum;
                    //然后去重
                    /*while (i < j && *i == *(i + 1)) {
                      ++i;
                      }
                      while (i < j && *j == *(j - 1)) {
                      ++j;
                      }*/
                }

            }
        }
        return retSum;
    }
};

int main(){
    /* vector<int> nums = {-1, 2, 1, -4,}; */
    vector<int> nums = {0,2,1,-3,};
    Solution sol;
    int ret = sol.threeSumClosest(nums,2);
    printf("ret:%d\n",ret);

    return 0;
}
