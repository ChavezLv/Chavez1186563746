#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:
    int func(int x){
        int countx = 0;
        while(x){
            countx++;
            x = x & (x - 1);
        }
        return countx;
    }
    
};
int main()
{
    Solution sol;
    cout << sol.func(9999) << endl;
    return 0;
}

