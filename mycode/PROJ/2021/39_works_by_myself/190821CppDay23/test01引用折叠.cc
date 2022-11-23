#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

template<typename _Tp>
class Solution {
public:

};
typedef int& lref;
typedef int&& rref;
void test01(){
    int n = 0;    
    lref& r1 = n;
    &r1;
    lref&& r2 = n;
    &r2;
    rref& r3 = n;
    &r3;

    rref&& r4 = 1;
    int && r5 = 1;
    &r4;
    cout << r4 << endl;
    cout << &r4 << endl;

}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

