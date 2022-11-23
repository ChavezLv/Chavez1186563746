#include <iostream>

using std::cout;
using std::endl;

class Point
{
public:

    //构造函数是可以进行重载的
    //
    //默认情况下，编译器会生成一个默认构造函数（无参构造函数）
    //
    //如果自定义构造函数，这个时候编译器就不会为我们生成默认的
    //（无参的）构造函数，如果此时还想调用无参构造函数，必须显示
    //的定义出来
    //
    Point() 
    : _ix(0)//真正初始化数据成员的位置，称为初始化表达式
    , _iy(0)//或者称为初始化列表
    {
        /* int a = 0; */
        /* int b(0);//也是初始化变量 */
        cout << "Point()" << endl;
        /* _ix = 0;//赋值 */
        /* _iy = 0; */
    }
    
    //构造函数的特点：完成数据成员的初始化
    Point(int ix, int iy)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int, int)" << endl;
        /* _ix = ix;//赋值 */
        /* _iy = iy; */
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

int main(int argc, char **argv)
{
    cout << "start test...." << endl;
    test();
    cout << "finish test...." << endl;
    return 0;
}

