#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class MyClass
{
public:
    MyClass(int i = 0)
    {
        cout << i;

    }
    MyClass(const MyClass &x)
    {
        cout << 2;

    }
    MyClass &operator=(const MyClass &x)
    {
        cout << 3;
        return *this;

    }
    ~MyClass()
    {
        cout << 4;

    }

};
int main()
{
    MyClass obj1(1), obj2(2);
    MyClass obj3 = obj1; 
    MyClass obj4;
    obj4 = obj1;
    cout << endl;
    return 0;

}
