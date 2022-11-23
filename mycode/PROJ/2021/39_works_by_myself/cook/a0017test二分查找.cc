#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    /* int binarySearch(vector<int>& vec, const int &target, const int &n){ */
    int search(vector<int>& nums, int target){
        int left = 0;
        int right = nums.size() - 1;
        if(nums.size() == 0){
            return -1;
        }
        if(target > nums[nums.size() - 1] || target <nums[0]){
            return -1;
        }
        while(left <= right){
            int mid = (left + right) / 2;
            if(nums[mid] == target){
                return mid;
            }else if(target < nums[mid]){
                right = mid - 1;
            }else if(target > nums[mid]){
                left = mid + 1;
            }
        }
        return -1;//未找到
    }
};

void test01(){
    vector<int> vec = {1, 3, 5, 7, 9};    
    cout << Solution().search(vec, 3) << endl;
}

int main(int argc, char* argv[])
{

    test01();
    return 0;
}
