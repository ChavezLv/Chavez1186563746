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

    Base(double dbase)
    :_dbase(dbase)
    {
        cout << "Base(double)" << endl;
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
    , _dderived(dderived)
    {
        cout << "Derived(double)" << endl;
    }
#endif
private:
    double _dderived;
};

int main(int argc, char **argv)
{
    Derived d(10);
    return 0;
}

