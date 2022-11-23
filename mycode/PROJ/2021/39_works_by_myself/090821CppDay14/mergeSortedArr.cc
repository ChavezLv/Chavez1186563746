#include <unistd.h>
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
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m + n - 1;
        int j = m - 1;
        int k = n - 1;
        /* for(i = m + n - 1; n > 0 && i >= 0; --i){ */
        for(;j >= 0 || k >= 0;){
            if(j < 0){
                nums1[i] = nums2[k--];
            }else if(k < 0){
                nums1[i] = nums1[j--];
            }else if(nums1[j] > nums2[k]){
                nums1[i] = nums1[j];
                --j;
                /* if(j < 0){ */
                /*     break; */
                /* } */
            }else{
                nums1[i] = nums2[k];
                --k;
                /* if(k < 0){ */
                /*     break; */
                /* } */
            }
            --i;
        }
        /* if(n > 0 && k < 0){ */
        /*     while(i >= 0){ */
        /*         nums1[--i] = nums1[j--]; */
        /*     } */
        /* }else if(n > 0){ */
        /*     while(i >= 0){ */
        /*         nums1[--i] = nums2[k--]; */
        /*     } */
        /* } */
    }
};
int main()
{
    vector<int> nums1 = {1,2,3,0,0,0};
    vector<int> nums2 = {2,5,6};
    /* vector<int> nums1 = {1}; */
    /* vector<int> nums2; */
    Solution().merge(nums1, 3, nums2, 3);
    for(auto &elem : nums1){
        cout << elem << " ";
    }
    cout << endl;
    return 0;
}

