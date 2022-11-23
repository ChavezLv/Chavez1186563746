#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Solution {
public:
    Solution(){

    }

    Solution(int){}

    void fun(){}
};
int main()
{
    Solution a(10);
    a.fun();
    Solution b();
    b.fun();
    return 0;
}

