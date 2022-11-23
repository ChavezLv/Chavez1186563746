#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

int i = 1;
class Solution {
public:
    Solution()
    : _fourth(_third)
    , _second(i++)
    , _first(i++)
      , _third(i++){
        _third = i;
      }

    void print(){
        printf("%p\n", this);
        cout << _first << _second << _third << _fourth << endl;
        cout << "result : " << 
            _first + _second + _third + _fourth
            << endl;
    }
private:
    int _first;
    int _second;
    int _third;
    int &_fourth;
};
int main()
{
    Solution sol, sol1;
    sol.print();
    sol1.print();
    return 0;
}

