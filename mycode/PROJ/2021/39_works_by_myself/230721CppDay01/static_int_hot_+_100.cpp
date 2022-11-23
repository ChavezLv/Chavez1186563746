#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:
    
};
int main()
{
    static int hot = 200;
    int &rad = hot;
    /* cout << rad << endl; */
    hot = hot + 100;
    cout << rad << endl;
    /* cout << hot << endl; */
    return 0;
}

