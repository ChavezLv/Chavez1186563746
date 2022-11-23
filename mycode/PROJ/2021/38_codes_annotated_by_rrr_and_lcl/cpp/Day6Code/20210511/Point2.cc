#include <iostream>

using std::cout;
using std::endl;

class Point
{
public:
    explicit
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }

    void print()
    {
        cout << "(" << this->_ix 
            << ", "  << this->_iy 
            << ")" <<  endl;
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }
private:
    int _ix;
    int _iy;
};

void test()
{
    /* string s1 = "hello";//string("hello"); */
    Point pt1(1, 3);
    cout << "pt1 = ";
    pt1.print();

    cout << endl;
    Point pt2 = 5;//Point(5, 0)
    cout << "pt2 = ";
    pt2.print();
}

int main(int argc, char **argv)
{
    test();

    return 0;
}
hello kiki
hello kiki
hello kiki
hello kiki
hello kiki
hello kiki
