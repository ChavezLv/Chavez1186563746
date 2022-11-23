#include <iostream>

using std::cout;
using std::endl;

class Line
{
public:
    virtual
    void distance();
};
class Base
{
    friend void Line::distance();
public:
    Base(long base = 0)
    : _base(base)
    {
        cout << "Base(long = 0)" << endl;
    }

    virtual 
    void print()
    {
        cout << "Base::_base = " << _base  << endl;
    }

    ~Base()
    {
        cout << "~Base()" << endl;
    }
private:
    long _base;
};

class Derived
: public Base
{
public:
    Derived(long base = 0, long derived = 0)
    : Base(base)
    , _derived(derived)
    {
        cout << "Derived(long = 0)" << endl;
    }

    /* void print();//从基类吸收过来的 */
    virtual
    void print() override //C++11
    {
        cout << "Derived::_derived = "  << _derived <<  endl;
    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
    }

private:
    long _derived;
};

//虚函数实现多态的原理：当基类定义一个虚函数的时候，就会在基类的对象的存储布局
//之中产生一个虚函数指针（vfptr)，该虚函数指针指向基类自己的虚函数表（虚表），虚表
//中存放的是虚函数的入口地址，当派生类继承基类的时候，就会在派生类对象的存储布局中
//产生一个虚函数指针（vfptr），该虚函数指针指向派生类自己的虚函数表（虚表），该虚表
//存放的是派生类自己的虚函数的入口地址
//
//
//多态被激活的五个条件：
//1、基类要定义虚函数
//2、派生类要重写（重定义、覆盖）基类中的虚函数
//3、创建派生类的对象
//4、基类的指针指向(引用绑定)派生类对象
//5、基类的指针(引用)调用虚函数
//
//动态多态与虚函数等价吗？
//不等价，动态多态的体现必须要有虚函数，但是有虚函数并不一定体现
//动态多态

void func(Base *pbase)
{
    pbase->print();//对于同一指令，针对不同对象，产生不一样的行为
}

int main(int argc, char **argv)
{
    cout << "sizeof(Base) = " << sizeof(Base) << endl;
    cout << "sizeof(Derived) = " << sizeof(Derived) << endl << endl;

    Base base(10);
    base.print();
    Derived derived(22, 33);
    derived.print();

    cout << endl << endl;
    //Base *pbase = &base;
    func(&base);
    //Base *pbase = &derived;
    func(&derived);

    return 0;
}
