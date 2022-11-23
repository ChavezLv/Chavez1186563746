#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base(double base = 0)
    : _dbase(base)
    {
        cout << "Base(double = 0) " << endl;
    }

    void print() const
    {
        cout << "Base::_dbase = " << _dbase << endl;
    }

    ~Base()
    {
        cout << "~Base()" << endl;
    }
private:
    double _dbase;
};

class Derived
: public Base
{
public:
    Derived(double base = 0, double derived = 0)
    : Base(base)
    , _dderived(derived)
    {
        cout << "Derived(double = 0, double = 0 )" << endl;
    }

    void show() const
    {
        cout << "Derived::_dderived = " << _dderived << endl;
    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
    }
private:
    double _dderived;
};

int main(int argc, char **argv)
{
    cout << "sizeof(Base) = " << sizeof(Base) << endl;//8
    cout << "sizeof(Derived) = " << sizeof(Derived) << endl << endl;//16

    Base base(11.11);
    base.print();

    cout << endl;
    Derived derived(22.22, 33.33);
    derived.show();

    cout << endl << "从派生对象向基类对象进行转化" << endl;
    //向上转型是没有问题的，C++语法是支持的
    base = derived;//ok,1、可以将派生类对象赋值给基类对象 
    base.operator=(derived);//const Base &rhs = derived

    cout << endl;
    Base &ref = derived;//ok,2、基类的引用可以绑定到派生类对象

    Base *pbase = &derived;//ok,3、基类的指针可以指向派生类的对象


#if 0
    cout << endl;
    cout << "基类的对象向派生类进行转换" << endl;
    Base base1(10);
    Derived derived1(20, 30);
    derived1 = base1;//error,1、不能从基类对象赋值给派生类对象
    derived1.operator=(base1);//const Derived &rhs = base1;

    Derived &ref2 = base1;//error,2、不能将派生类的引用绑定到基类对象

    Derived *pderived1 = &base1;//error,3、不能将派生类指针指向基类对象
#endif
    Base base1(10);
    Derived derived1(20, 30);
    //不安全的向下转型
    Derived *pderived1 = static_cast<Derived *>(&base1);//向下转型：C++语法不支持，如果一定要转换，可以使用强制转换
    //安全的向下转型
    Base *pbase2 = &derived1;
    Derived *pderived2 = static_cast<Derived *>(pbase2);
    return 0;
}











