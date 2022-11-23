#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
/* using namespace std; */
using std::cin;
using std::string;
using std::cout;
using std::endl;

class Solution {
public:


};


void test(){

    int num;
    while(cin >> num, !cin.eof()){
        if(cin.bad()){
            return;
        }else if (cin.fail()){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "cin.fail" << endl;
        }else{
            cout << num << endl;
        }
    }
}
int main()
{
    test();

    return 0;
}

