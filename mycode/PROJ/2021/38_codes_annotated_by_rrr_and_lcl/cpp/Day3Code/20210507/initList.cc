#include <iostream>

using std::cout;
using std::endl;


class Test
{
public:
    Test(int value)
    /* : _ix(value) */
    /* , _iy(_ix) */
    : _iy(value)//数据成员的初始化顺序与其在初始化列中的顺序没有关系
    , _ix(_iy)//与数据成员再被声明的时候有关
    {
        cout << "Test(int)" << endl;
    }

    void print()
    {
        cout << "ix = " << _ix << endl
             << "iy = " << _iy << endl;
    }
private:
    int _iy;
    int _ix;
};

int main(int argc, char **argv)
{
    Test test1(10);
    test1.print();
    return 0;
}

