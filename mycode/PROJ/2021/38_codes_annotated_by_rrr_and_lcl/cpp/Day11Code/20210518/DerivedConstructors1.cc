#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base()
    :_dbase(0)
    {
        cout << "Base()" << endl;
    }
private:
    double _dbase;
};

class Derived
: public Base
{
public:
    Derived(double dderived)
    : Base()
    , _dderived(dderived)
    {
        cout << "Derived(double)" << endl;
    }
private:
    double _dderived;
};

int main(int argc, char **argv)
{
    //error,创建派生对象的时候，先调用基类构造函数，然后调用派生类构造函数
    //ok,创建派生类对象的时候，会调用派生类的构造函数，为了完成对从基类
    //继承的数据成员的初始化，所以需要利用基类的构造函数，然后在调用派生的
    //构造函数的函数体
    Derived d(10);
    return 0;
}

