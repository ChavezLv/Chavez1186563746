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

private:
    Solution(){
        cout << "Solution()" << endl;
    }
    ~Solution(){
        cout << "~Solution()" << endl;
    }
    int print(){
        cout << "123" << endl;
        return 1;
    }

    static int priint;
    static Solution *sol;
    static Solution *sol1;
};

/* int Solution::priint = print();//静态数据成员初始化不能调用私有函数返回值 */
Solution* Solution::sol = new Solution();//静态数据成员
/* Solution* Solution::sol1 = new Solution(); */

int main()
{
    
    return 0;
}

