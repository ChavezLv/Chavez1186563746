#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <algorithm>
#include <functional>

using namespace std;
/* template<typename _Tp> */
class Solution {
public:
    function<void(int &, int&)> swap = [](int& lhs, int& rhs){
        int temp = lhs;
        lhs = rhs;
        rhs = temp;
    };
    int findRepeatNumber(vector<int>& nums) {
        //整体思路就是: < 返回 = 继续++, !+则一直替换, 并且替换前判断一下一样就返回
        for(int index = 0; index < (int)nums.size(); ++index){
            if(nums[index] < index){
                //因为index 走到哪都会确保走过的地方是正确的
                return nums[index];
            }
            while(nums[index] != index){
                /* cout << nums[index] << endl; */
                if(nums[index] == nums[nums[index]]){
                    //必须在swap之前进行判断, 否则刚交换完成比如nums[0] = 0
                    //这时候就退出了!
                    return nums[index];
                }
                swap(nums[index], nums[nums[index]]);
                /* if(nums[index] == nums[nums[index]]){ */
                /*     return nums[index]; */
                /* } */
            }
        }
        return -1;
    }
   
};
void test01(){
    /* vector<int> vec = {2, 3, 1, 0, 2, 5, 3}; */       
    vector<int> vec = {2, 3, 1, 0, 2, 5, 3};       
    /* vector<int> vec = {2, 3, 1, 0, 2, 5, 3}; */       
    cout << Solution().findRepeatNumber(vec) << endl;
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}
