#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

class Point;//类的前向声明

class Line
{
public:
    float distance(const Point &lhs, const Point &rhs);
    /* { */
    /*     return hypot(lhs._ix - rhs._ix, lhs._iy - rhs._iy); */
    /* } */

    //重载函数如果想都作为另外一个类的友元，需要分别设置成类的友元
    float distance(Point &pt, int x, int y);
};
class Point
{
    //友元是不受public/private/protected关键字控制
    friend float Line::distance(const Point &lhs, const Point &rhs);
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }

    void print() const
    {
        cout << "("   << _ix 
             << ", "  << _iy 
             << ")"   <<  endl;
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }

private:
    int _ix;
    int _iy;
};

//2、友元之成员函数
float Line::distance(const Point &lhs, const Point &rhs)
{
    return hypot(lhs._ix - rhs._ix, lhs._iy - rhs._iy);
}

float Line::distance(Point &pt, int x, int y)
{
    pt._ix = x;
}
void test()
{
    Point pt1(1, 2);
    Point pt2(4, 6);
    cout << "pt1 = ";
    pt1.print();
    cout << "pt2 = ";
    pt2.print();
    cout << "pt1与pt2之间的距离: " << Line().distance(pt1, pt2) << endl;
    //Line()创建临时对象/匿名对象,临时对象的创建与销毁都在本行
}

int main(int argc, char **argv)
{
    test();

    return 0;
}
