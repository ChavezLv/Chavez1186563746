#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Solution {
    int _a1, _a2;
public:
    Solution(int x1 = 0, int x2 = 0);
    /* Solution(int x1 = 0, int x2 = 0){ */
    /*     _a1 = x1; */
    /*     _a2 = x2; */
    /* } */
    void a1(){
        cout << _a1 << endl; 
    }
    void a2(){
        cout << _a2 << endl; 
    }
};
Solution::Solution(int x1, int x2){
    
}
int main()
{
    Solution data(2, 3);
    data.a1();
    data.a2();
    return 0;
}

