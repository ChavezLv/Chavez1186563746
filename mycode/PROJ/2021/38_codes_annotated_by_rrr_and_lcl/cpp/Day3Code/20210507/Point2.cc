#include <iostream>

using std::cout;
using std::endl;

class Point
{
public:

    //构造函数的特点：完成数据成员的初始化
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }

    //默认情况下，编译器会自动生成拷贝构造函数
    Point(const Point &rhs)//Point pt2 = pt1
    : _ix(rhs._ix)
    , _iy(rhs._iy)
    {
        cout << "Point(const Point &)" << endl;
    }

    void print()
    {
        cout << "(" << _ix << ", "
              << _iy << ")" <<  endl;
    }

    //析构函数的作用：完成数据成员的清理工作
    //析构函数没有返回类型，没有参数，所以具有唯一性，不能重载
    //对象在离开其作用域被销毁的时候，会自动调用析构函数
    //
    //默认情况下，编译器会自动生成析构函数
    ~Point()
    {
        cout << "~Point()" << endl;
        //print();
    }
private:
    int _ix;
    int _iy;
};

void test()
{
    //int a;
    /* Computer com; */
    Point pt1;//栈对象
    pt1.print();

    cout << endl;
    Point pt(1, 3);//对象创建的时候会自动调用构造函数
    /* pt.Point(1, 3);//error,构造函数不能以对象加点的形式进行调用 */
    /* Point(2, 5).print();//ok,构造函数的调用会创建对象 */

    pt.~Point();//析构函数是可以显示调用的,一般不建议显示调用

    cout << "pt = ";
    pt.print();
}

void test2()
{
    int a(3);
    int b = a;

    Point pt1(1, 3);
    cout << "pt1 = ";
    pt1.print();

    cout << endl;
    Point pt2 = pt1;//pt2对象创建会有构造函数的调用
    cout << "pt2 = ";
    pt2.print();

}
int main(int argc, char **argv)
{
    cout << "start test...." << endl;
    test2();
    cout << "finish test...." << endl;
    return 0;
}

