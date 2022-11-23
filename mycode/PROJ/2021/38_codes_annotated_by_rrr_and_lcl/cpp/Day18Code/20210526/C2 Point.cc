#include <iostream>

using std::cout;
using std::endl;
//course2 
// 笔记by lcl:\
C1学习了 普通函数模板, C2 学习成员函数模板\
1) template <typename T>\
    T func(){}\
    cout << "pt.func() = " << pt.func() << endl;//error, 因为func()参数是空的, 推不了\
    所以要显示实例化,不能写成隐式实例化\
\
    template <typename T = int>\
    pt.func();//ok, 因为此时的T默认都是int了\
2) 模板是静态多态, 编译时, 重载也是; 只有多态时运行的, 动态多态

//接下来学习可变模板参数
class Point
{
public:
    Point(double dx = 0, double dy = 0)
    : _dx(dx)
    , _dy(dy)
    {
        cout << "Point(double = 0, double = 0)" << endl;
    }

    template <typename T = int>
    T func()
    {
        return (T)(_dx);//强转
    }
private:
    double _dx;
    double _dy;
};
int main(int argc, char **argv)
{
    Point pt(1.1, 2.2);
    cout << "pt.func() = " << pt.func<int>() << endl;
    cout << "pt.func() = " << pt.func() << endl;//因为有默认参数了, 所以可以这样写
    return 0;
}

