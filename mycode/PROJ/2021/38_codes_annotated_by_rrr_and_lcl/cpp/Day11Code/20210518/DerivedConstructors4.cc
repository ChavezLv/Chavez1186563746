#include <iostream>

using std::cout;
using std::endl;

class Test
{
public:
    Test()
    {
        cout << "Test()" << endl;
    }

    ~Test()
    {
        cout << "~Test()" << endl;
    }
};
class Base
{
public:
#if 0
    Base()
    :_dbase(0)
    {
        cout << "Base()" << endl;
    }
#endif

    Base(double dbase)
    :_dbase(dbase)
    {
        cout << "Base(double)" << endl;
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
#if 0
    Derived()
    : Base()
    , _dderived(0)
    {
        cout << "Derived(0)" << endl;
    }
#endif
#if 1
    Derived(double dderived)
    : Base(dderived)
    , _tst()
    , _dderived(dderived)
    {
        cout << "Derived(double)" << endl;
    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
    }
#endif
private:
    double _dderived;
    Test _tst;
};

int main(int argc, char **argv)
{
    Derived d(10);
    return 0;
}

