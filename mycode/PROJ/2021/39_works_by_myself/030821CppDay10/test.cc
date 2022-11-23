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

class Point {
public:
    Point(const int &x, const int &y)
    : _x(x)
    , _y(y){

    }

    operator int() {
        cout << "operator int(){}" << endl;
        return _x + _y; 
    }
    /* operator char(){ */
    /*     return _x + _y; */
    /* } */

private:
    int _x;
    int _y;
};

int main()
{
    string s1("hello");
    s1[0] = 97;
    
    cout << s1 << endl;
    //重载了类型转换函数
    
    Point p1(1,2);
    int a = p1;
    float c = p1;
    double b = p1;
    char d = p1;
    cout << d << endl;
    cout << p1 << endl;
    return 0;
}

