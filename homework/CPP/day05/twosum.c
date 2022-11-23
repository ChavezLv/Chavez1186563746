#include<iostream>
#include<vector>
using namespace std;
class Solution{
public:
    vector<int> twoSum(vector<int>& nums,int target){
        int n=nums.size();
        for(int i=0;i<n;++i){
            for(int j=i+1;j<n;++j){
                if(nums[j]+nums[j]==target){
                    return {i,j};
                }
            }
        }
        return {};
    }
};
//Hash
class Solution2{
public:
    vector<int> twoSum(vector<int>& nums,int target){
        unorderd_map<int,int>hashtable;
        for(int i=0;i<nums.size();++i){
            auto it=hashtable.find(target-nums[i]);
            if(it!=hashtable.end()){
                return {it->second,i};
            }
            hashtable[nums[i]]=i;
        }
        return {};
    }
};
