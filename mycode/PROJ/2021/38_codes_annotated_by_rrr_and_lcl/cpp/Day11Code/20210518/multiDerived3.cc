#include <iostream>

using std::cout;
using std::endl;

class A
{
public:
    A(double dx = 0)
    : _dx(dx)
    {
        cout << "A(double = 0)" << endl;
    }

    void setX(double x)
    {
        _dx = x;
    }

    void print() const
    {
        cout << "A::_dx = " << _dx << endl;
    }

    ~A()
    {
        cout << "~A()" << endl;
    }
private:
    double _dx;
};

//加了虚拟继承之后，就会在派生类的对象的存储布局之中存在
//一个虚基指针
class B
: virtual public A
{
public:
    B()
    : A()
    {
        cout << "B()" << endl;
    }

};

class C
: virtual public A
{
public:
    C()
    : A()
    {
        cout << "C()" << endl;
    }
};

class D
: public B
, public C
{
public:
    D()
    {
        cout << "D()" << endl;
    }

    ~D()
    {
        cout << "~D()" << endl;
    }
};

int main(int argc, char **argv)
{
    //多基继承的问题2：存储二义性（_dx)
    //解决办法：虚拟继承
    cout << "szieof(A) = " << sizeof(A) << endl;//8
    cout << "szieof(B) = " << sizeof(B) << endl;//8
    cout << "szieof(C) = " << sizeof(C) << endl;//8
    cout << "szieof(D) = " << sizeof(D) << endl;//16
    cout << endl;

    D d;

    /* d.A::print();//error */
    /* d.A::setX(10); */

    cout << endl;
    d.B::setX(20);
    d.B::print();

    cout << endl;
    d.C::setX(30);
    d.C::print();

    cout << endl;
    d.setX(4000);
    d.print();

    d.print();
    return 0;
}

