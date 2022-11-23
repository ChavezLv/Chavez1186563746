#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    int jump(vector<int> & nums){
        int curDistance = 0;
        int ans = 0;
        int nextDistance = 0;
        for(int i = 0; i < (int)nums.size() - 1; ++i){
            nextDistance = max(nums[i] + i, nextDistance);
            if(i == curDistance){
                curDistance = nextDistance;
                ++ans;
            }
        }
        return ans;
    
    }
   
};

void test01(){
    vector<int> vec = {2, 3, 1, 1, 4};
    cout << Solution().jump(vec) << endl;
}

int main(int argc, char* argv[])
{
    test01();
    return 0;
}
