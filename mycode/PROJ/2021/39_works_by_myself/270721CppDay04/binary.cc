#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;

class Solution {
public:
    static void binarySearch(int num){
        int cnt = 0;
        for(int i = 0, spl = 1; i < 32; ++i, spl <<= 1){
            cout << "spl:" << spl << endl;
            if(num & spl){
                ++cnt;
            }
        }
        cout << cnt << endl;
    }
};
int main()
{
    int num;
    cin >> num;
    Solution::binarySearch(num);
    return 0;
}

