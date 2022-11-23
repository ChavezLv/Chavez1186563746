#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base(const char *base)
    : _base(new char[strlen(base) + 1]())
    {
        cout << "Base(const char *)" << endl;
        strcpy(_base, base);
    }

    virtual
    void print()
    {
        cout << "Base::_base = " << _base << endl;
    }

    //基类的析构函数被设置为虚函数之后，派生类的析构函数
    //自动变为虚函数
    //
    //为什么可以这么做？编译器做了一个优化，因为析构函数对于任何
    //一个类只有一个，具有唯一性，编译就将析构函数改为destructor
    //,当基类将析构函数定义为虚函数之后，只要派生类定义了自己
    //析构函数，编译器就认为这是一种重写(重定义)

    //destructor
    // 虚析构函数就是解决内存泄漏的问题, main函数申请的
    // 因为一个类的析构函数只有一个, 编译器优化时 就
    //能解释成~destructor, 这是对编译器而言名字是相同的, 
    //所以就满足了多态的条件2: 重写;

    //即当基类的析构函数 被设计为虚函数的时候, 派生的析构函数
    //会自动变成虚函数. 此时虽然在用户看来, 名字是不一样的,
    // 但编译器看到的是一样的. 
    virtual
    ~Base()
    {
        cout << "~Base()" << endl;
        if(_base)
        {
            delete [] _base;
            _base = nullptr;
        }
    }
private:
    char *_base;
};

class Derived
: public Base
{
public:
    Derived(const char *base, const char *derived)
    : Base(base)
    , _derived(new char[strlen(derived) + 1]())
    {
        cout << "Derived(const char *)" << endl;
        strcpy(_derived, derived);
    }

    void print() override
    {
        cout << "Derived::_derived = " << _derived << endl;
    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
        if(_derived)
        {
            delete [] _derived;
            _derived = nullptr;
        }
    }

private:
    char *_derived;

};
int main(int argc, char **argv)
{
    Base *pbase = new Derived("hello", "world");
    pbase->print();

    /* delete dynamic_cast<Derived *>(pbase); //不完美的解决方案*/
    delete pbase;//1、析构函数
    //pbase->~Base();
    //pbase->~destructor编译器解释成了这样!

    return 0;
}

