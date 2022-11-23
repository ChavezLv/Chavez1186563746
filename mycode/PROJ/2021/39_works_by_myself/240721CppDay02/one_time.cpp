#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
class Solution {
public:
    int findNum(vector<int>& nums){
        int ret = 0;
        int cnt = 0;
        sort(nums.begin(),nums.end());
        for(auto iter = nums.cbegin(); iter != nums.cend(); ++iter){
            if(iter == nums.cbegin()){
                cnt = 1;
            }else{
                if(*iter == *(iter - 1)){
                    ++cnt;
                }else{
                    if(cnt != 3 || (iter + 1) == nums.cend()){
                        ret = *iter;
                        break;
                    }
                    cnt = 1;
                }
            } 
            cout << "for: " << cnt <<endl;
        }

        return ret;
    }
    int findNumByBit(vector<int>& nums){
        int *bitMap = new int[32];//int *bitMap = new int[32];
        int spl = 1;
        for(int i = 0; i < 32; ++i){
            for(int j = 0; i < (int)nums.size(); ++i){
                cout << (nums[j] & spl) << endl;
                if((nums[j] & spl) == 1){
                    ++bitMap[i];
                }
            }
            spl <<= 1;
        }
        int num = 0;
        for(int i = 0; i < 32; ++i){
            /* cout << bitMap[i] << endl; */
            bitMap[i] = bitMap[i] % 3;
            num += bitMap[i] * pow(2, i); 
        }

        
        return num;
    }
};
int main()
{
    vector<int> nums = {1,2,1,1,2,3,2};
    Solution sol;
    /* cout << sol.findNum(nums) << endl; */
    cout << sol.findNumByBit(nums) << endl;
    return 0;
}

