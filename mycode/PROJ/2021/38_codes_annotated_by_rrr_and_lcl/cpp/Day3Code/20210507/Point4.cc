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

    //Q:拷贝构造函数参数中的引用符号可以去掉吗？
    //A:不能去掉，如果去掉引用符号，在满足拷贝构造函数的
    //情况，会满足拷贝构造函数的调用时机1，然后进行无限循环调用
    //拷贝构造函数，函数的参数是会入栈，这样的话栈空间肯定会溢出
    //的，所以引用符号不能去掉
    //
    //Q：拷贝构造函数参数中的const可以去掉吗？
    //A:不能去掉，当传递的是右值（临时对象、临时变量、匿名对象）
    //的时候，会返回报错（非const左值引用不能绑定到右值），当传递
    //临时对象的时候，就会报错
    //
    //默认情况下，编译器会自动生成拷贝构造函数
    /* Point(const Point &rhs)//Point pt2 = pt1 */
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
    //拷贝构造函数调用时机1：用一个已经存在的对象初始化一个
    //刚刚创建的对象时候，会调用拷贝构造函数
    //
    //
    //Point(const Point &rhs)
    //const Point &rhs = pt1
    //const Point rhs = pt1;
    //const Point rhs = pt1;
    //const Point rhs = pt1;
    //const Point rhs = pt1;
    //const Point rhs = pt1;
    Point pt2 = pt1;//pt2对象创建会有构造函数的调用
    cout << "pt2 = ";
    pt2.print();

}

//拷贝构造函数的调用时机2：当函数的参数是类类型的时候，
//在调用函数的时候，进行形参与实参结合的时候，会调用
//拷贝构造函数
void func2(Point pt)//Point pt = pt3;
{
    pt.print();
}

void test3()
{
    Point pt3(10, 30);
    cout << "pt3 = ";
    pt3.print();

    cout << endl << endl;
    func2(pt3);
}

int func0()
{
    int a = 10;
    return a;
}

//拷贝构造函数的调用时机3：当函数的返回类型是类类型的时候，
//在执行return语句的时候会执行拷贝构造函数
Point func3()
{
    Point pt1(1, 4);
    cout << "pt1 = ";
    pt1.print();

    return pt1;
}

void test4()
{
    int number = 100;
    int &ref = number;
    const int &ref2 = 100;
    /* &100;//error,字面值常量 */
    &number;

    //Point(Point &rhs)
    /* &func3();//error */
    const Point &rhs = func3();
    Point pt2 = func3();
    cout << "pt2 = ";
    pt2.print();
}
int main(int argc, char **argv)
{
    test4();
    return 0;
}

