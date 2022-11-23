#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Solution {
public:
    ~Solution(){
        cout << "Solution destuctor\n" ;
    }
};
class Solution2 {
public:
    ~Solution2(){
        cout << "Solution2 destuctor\n" ;
    }
};
class Solution5 {
public:
    ~Solution5(){
        cout << "Solution5 destuctor\n" ;
    }
};
class Solution3 {
public:
    ~Solution3(){
        cout << "Solution3 destuctor\n" ;
    }
};
class Solution4{
public:
    ~Solution4(){
        cout << "Solution4 destuctor\n" ;
    }
};
Solution3 c;
Solution4 g;
int main()
{
    Solution *a = new Solution();
    static Solution4 d;
    static Solution5 f;
    Solution2 b;
    Solution3 e;
    delete a;

    return 0;
}

