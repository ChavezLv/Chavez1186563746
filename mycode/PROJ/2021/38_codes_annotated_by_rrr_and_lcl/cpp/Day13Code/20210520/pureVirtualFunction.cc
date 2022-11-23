#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    //声明了纯虚函数的类称为抽象类，抽象类不能创建对象
    virtual
    void print() = 0;//纯虚函数
    //纯虚函数的实现是交给派生类去做的

    virtual
    void show() = 0;
};

//如果抽象类的派生类没有实现抽象类中所有的纯虚函数，那么派生类也是
//一个抽象类，也不能创建对象
//即派生类继承自抽象类, 但是派生类没有将抽象类里面的纯虚函数全部实现, 
//依旧是抽象类, 不能创建对象.
class Derived
: public Base
{
public:
    virtual
    void print() override
    {
        cout << "Derived::print()" << endl;
    }
};

class Derived2
: public Derived
{
public:
    virtual
    void print() override
    {
        cout << "Derived2::print()" << endl;
    }

    virtual
    void show() override
    {
        cout << "Derived2::show()" << endl;
    }
};

int main(int argc, char **argv)
{
    /* Base base;//error */
    /* Derived derived;//error */
    /* derived.print(); */

    Derived2 d2;
    d2.print();
    d2.show();

    cout << endl;
    Base *pbase = &d2;
    Derived *pderived = &d2;
    pbase->show();
    pbase->print();

    pderived->show();
    pderived->print();

    return 0;
}






