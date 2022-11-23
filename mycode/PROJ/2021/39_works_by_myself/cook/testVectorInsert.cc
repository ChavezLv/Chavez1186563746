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
void printVec(const std::vector<int>&vec){
    for(auto &i : vec){
        cout << i << " ";
    }
    cout << endl;
}
//vector都是指定位置插入
int main()
{
    std::vector<int> v{1,2,3,1,2,3,3,4,5,4,5,6,7};
    std::sort(v.begin(), v.end());
    printVec(v);

    auto last = std::unique(v.begin(), v.end());
    //1 2 3 4 5 6 7 4 4 5 5 6 7
    /* cout << *last << endl;//last == 4 */
    v.erase(last, v.end());
    printVec(v);
    
    /* v.insert(v.begin(), 200);//v.begin()前插入 */
    /* printVec(v);// 200 1 2 3 4 5 6 7 */
    
    /* v.insert(v.begin(), 2, 300);*/
    /* printVec(v);//300 300 1 2 3 4 5 6 7 */

    vector<int> v2(2, 315);//{315, 315}
    v.insert(v.begin() + 2, v2.begin(), v2.end());
    printVec(v);//1 2 315 315 3 4 5 6 7

    return 0;
}

