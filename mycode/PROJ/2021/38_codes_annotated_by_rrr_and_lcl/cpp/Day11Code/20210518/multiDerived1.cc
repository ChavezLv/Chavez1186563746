#include <iostream>

using std::cout;
using std::endl;

class A
{
public:
    A()
    {
        cout << "A()" << endl;
    }

    void print() const
    {
        cout << "A::print() const" << endl;
    }

    ~A()
    {
        cout << "~A()" << endl;
    }

};

class B
{
public:
    B()
    {
        cout << "B()" << endl;
    }

    void show() const
    {
        cout << "B::show() const" << endl;
    }

    ~B()
    {
        cout << "~B()" << endl;
    }

};

class C
{
public:
    C()
    {
        cout << "C()" << endl;
    }

    void display() const
    {
        cout << "C::display() const" << endl;
    }

    ~C()
    {
        cout << "~C()" << endl;
    }

};

//对于多基继承，每一个基类前面都要写继承方式，否则就是默认的（private）
//
//派生类多基继承基类的时候，基类构造函数的调用顺序只与基类被派生类
//继承的先后顺序有关，与基类在派生类构造函数的初始化列表中的顺序没有关系
//
class D
: public A
, public B
, public C
{
public:
    D()
    : A()
    , B()
    , C()
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
    D d;
    d.print();
    d.show();
    d.display();
    return 0;
}

