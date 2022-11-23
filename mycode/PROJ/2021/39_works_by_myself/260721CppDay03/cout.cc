#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Solution {
public:

};
int main()
{
    /* operator<<(cout, "helloworld\n"); */
    /* cout.operator<<("helloworld");//返回一个地址 */
    /* operator<<(cout, "hello") << endl; */
    operator<<(cout, "hello").operator<<(endl);
    return 0;
}

