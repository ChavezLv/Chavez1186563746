#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

class Point;//类的前向声明

//友元的性质：友元是单向的, 友元是没有传递性的（A->B->C）,友元不能被继承
class Line
{
public:
    float distance(const Point &lhs, const Point &rhs);
    void setPoint(Point &pt, int x, int y);
private:
    int _iz;
};

class Point
{
    //友元是不受public/private/protected关键字控制
    /* friend float Line::distance(const Point &lhs, const Point &rhs); */
    /* friend void Line::setPoint(Point &pt, int x, int y); */
    //3、友元之友元类
    friend class Line;
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }
#if 0
    void setLine(Line &line, int z)//error
    {
        line._iz = z;
    }
#endif

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

void Line::setPoint(Point &pt, int x, int y)
{
    pt._ix = x;
    pt._iy = y;
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
