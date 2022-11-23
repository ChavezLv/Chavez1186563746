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

};
int main()
{
    char a[200];
    /* cin.getline(a, 200, '\n');//默认一行 */
    /* cin.getline(a, 200, '/');//指定以什么结束 '/' */
    cin.getline(a, 200, 'a');//指定以什么结束 '/'
    /* cin.getline(a, 200);//默认一行 */
    cout << a;
    return 0;
}

