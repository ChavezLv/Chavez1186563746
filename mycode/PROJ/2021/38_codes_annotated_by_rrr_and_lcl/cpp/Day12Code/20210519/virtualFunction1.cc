#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base(long base = 0)
    : _base(base)
    {
        cout << "Base(long = 0)" << endl;
    }

    virtual 
    void print()
    //void print() const
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
    //virtual//可写可不写
    void print()
    //void print() const override// 重写, 重定义, 需要有虚函数virtual 才能加一个override(可加可不加)
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

void func(Base *pbase)
{
    pbase->print();//对于同一指令，针对不同对象，产生不一样的行为
}

int main(int argc, char **argv)
{
    cout << "sizeof(Base) = " << sizeof(Base) << endl;//  8 -> 16
    cout << "sizeof(Derived) = " << sizeof(Derived) << endl << endl;// 16 -> 24

    Base base(10);
    base.print();
    Derived derived(22, 33);
    derived.print();

    cout << endl << endl;
    func(&base);//Base *pbase = &base1;
    func(&derived);//Base *pbase = &derived1;
    return 0;
}

