#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    vector<bool> isEquelGapArray(vector<vector<int> > & arrs){
        for(auto &nums: arrs){
            int size = nums.size();
            int max = INT32_MIN;
            int min = INT32_MAX;
            int sum = 0;
            for(auto &num : nums){
                sum += num;
                max = max > num ? max : num;
                min = min < num ? min : num;
            }
            /* cout << sum << " " << min << " " << max << " " << nums.size() << endl; */
            /* cout << ((min + max) * size) / 2 << endl; */
            /* cout << (min + max) * nums.size() / (float)2 << endl; */
            //0000  1111 之类的
            if(max == min){
                vec.push_back(true);
                continue;
            }
            //sum不需要判断
            /* if(sum != (min + max) * size / (float)2){ */
            /*     vec.push_back(false); */
            /*     continue; */
            /* } */

            //公差不为整数的
            //因为整数等差数组的公差不会是浮点的,所以不需要使用int
            if((max - min) % (size - 1) != 0){
                vec.push_back(false);
                continue;
            }
            
             
            /* if((max - min) / (float)(size - 1) != (max - min) / (size - 1)){ */
            /*     vec.push_back(false); */
            /* } */
            int commonGap = (max - min) / (size - 1);
                /* cout << commonGap << endl; */
            int index = 0;

            
            pair<unordered_set<int>::iterator, bool> it;
            unordered_set<int> exist;
            for(; index < (int) nums.size(); ++ index){
                it = exist.insert(nums[index]); 
                if(!it.second){
                    break;
                }
                if((nums[index] - min) % commonGap != 0){
                    break;
                }
            }

            if(index != size || it.second == false){
                vec.push_back(false);
                continue;
            }else{
                vec.push_back(true);

            }
        }
        return vec;
    }
    
    int maxProfit(vector<int> &nums){
        int maxProf = 0;
        for(int index = 1; index < (int)nums.size(); ++index){
            maxProf += max(nums[index] - nums[index - 1], 0);
        }
        return maxProf;
    }
private:
    vector<bool> vec;
   
};
void test01(){
    vector<int> nums = {1, 4, 2, 3};
    cout << Solution().maxProfit(nums) << endl;
}
void test02(){
    vector<vector<int> > arrs = {{3, 5, 1, 7}, {0, 1, 3}, {2, 6, 7, 1},\
        {-68,-96,-12,-40,16}, {0,5,3,4,6,9}, {1,2,2,5,5}, {1, 1, 1, 1}};
    vector<bool> vec = Solution().isEquelGapArray(arrs);
    for_each(vec.begin(), vec.end(), [](const bool & elem ){
                cout << elem << " ";
             });
    cout << endl;

}
void test03(){
    char p[] = {1, 2, 3, 4, 5};
    printf("%d, %lu, %lu\n", p[0], strlen(p), sizeof(p));//1, 5, 5 

    char str1[] = "abc";
    printf("%d, %lu, %lu\n", str1[0], strlen(str1), sizeof(str1));// 97, 3, 4

    char str2[] = {'a', 'b', 'c'};
    printf("%d, %lu, %lu\n", str2[0], strlen(str2), sizeof(str2));//49, 6, 3// 可能error

    char str3[] = {97, 'b', 'c', 'd', 'e'};
    printf("%d, %lu, %lu\n", str3[0], strlen(str3), sizeof(str3));//97, 5, 5//maybe error

    char str4[] = {97, '\0', 'c', 'd', 'e'};
    printf("%d, %lu, %lu\n", str4[0], strlen(str4), sizeof(str4));//97, 1, 5

    char str5[] = {97, 0, 'c', 'd', 'e'};
    printf("%d, %lu, %lu\n", str5[0], strlen(str5), sizeof(str5));//97, 1, 5

    char str6[] = "abcde";
    printf("%d, %lu, %lu\n", str6[0], strlen(str6), sizeof(str6));//97, 5, 6

    char str7[] = "a\0cde";
    printf("%d, %lu, %lu\n", str7[0], strlen(str7), sizeof(str7));//97, 1, 6
}
int main(int argc, char* argv[])
{
    /* char p[] = {0, 1, 3, 4, 5}; */
    /* char *p1 = {0}; */
    /* char p[] = {'0'}; */
    /* printf("%d, %lu, %lu\n", p[0], strlen(p), sizeof(p)); */
    /* printf("%d, %lu, %lu\n", p1[0], strlen(p1), sizeof(p1)); */
    /* cout << sizeof(p) << " " << strlen(p) <<  endl; */
    /* test01(); */
    test02();
    test03();
    /* unsigned long a = 5; */
    /* int b = -96; */
    /* int c = 16; */
    /* cout << a + b << endl; */
    /* cout << a * b << endl; */
    /* cout << (b + c)  * a/ (float)2 << endl; */
    return 0;
}
