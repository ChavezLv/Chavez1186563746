#include "Point.h"
#include <iostream>

using std::cout;
using std::endl;


class Line::PointPimpl//定义应该【即使是私有类也要这么定义】
{
public:
    PointPimpl(int x1, int y1, int x2, int y2)
    : _pt1(x1, y1)//类对象成员需要显示进行初始化，否则就是默认值
    , _pt2(x2, y2) //特别是自定义类型的尤其要自己初始化
    {
        cout << "PointPimpl(int, int, int,int)" << endl;
    }

    void printPimpl()
    {
        _pt1.print();
        _pt2.print();
    }

    ~PointPimpl()
    {
        cout << "~PointPimpl()" << endl;
    }
private:
    class Point
    {
    public:
        Point(int ix = 0, int iy = 0)
        : _ix(ix)
        , _iy(iy)
        {
            cout << "Point(int = 0, int = 0)" << endl;
        }
    
        void print()
        {
            cout << "(" << _ix << ", "
                  << _iy << ")" <<  endl;
        }
    
        ~Point()
        {
            cout << "~Point()" << endl;
        }
    private:
        int _ix;
        int _iy;
    };

private:
    Point _pt1;//类对象成员(子对象)
    Point _pt2;
};

Line::Line(int x1, int y1, int x2, int y2)//最上层【抽象层】的【构造函数】
: _pimpl(new PointPimpl(x1, y1, x2, y2))//唯一数据成员【桥梁指针pImpl】：
//1-new [实现实体] 2-接收返回的指针
{
    cout << "Line(int, int, int, int)" << endl;
}

void Line::printLine()
{
    _pimpl->printPimpl();//设计实现就调用别的接口
}
//cpp工程的编译能用makefile改吗
Line::~Line()
{
    cout << "~Line()" << endl;
    if(_pimpl)//析构函数1-防止double_free的检查，如果_pimpl不为空才进入
    {
        delete _pimpl;
        _pimpl  = nullptr;//2-删除以后立即置空//和1配合使用
    }
}
