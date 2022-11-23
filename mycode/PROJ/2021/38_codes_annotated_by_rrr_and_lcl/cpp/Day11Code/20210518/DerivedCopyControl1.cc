#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base()
    : _pbase(nullptr)
    {
        cout << "Base()" << endl;
    }

    Base(const char *pbase)
    : _pbase(new char[strlen(pbase) + 1]())
    {
        cout << "Base(const char *)" << endl;
        strcpy(_pbase, pbase);
    }

    Base(const Base &rhs)
    : _pbase(new char[strlen(rhs._pbase) + 1]())
    {
        cout << "Base(const Base &)" << endl;
        strcpy(_pbase, rhs._pbase);
    }

    Base &operator=(const Base &rhs)
    {
        cout << "Base &operator=(const Base &)" << endl;
        if(this != &rhs)
        {
            delete [] _pbase;
            _pbase = nullptr;

            _pbase = new char[strlen(rhs._pbase) + 1]();
            strcpy(_pbase, rhs._pbase);
        }

        return  *this;
    }

    ~Base()
    {
        cout << "~Base()" << endl;
        if(_pbase)
        {
            delete [] _pbase;
            _pbase = nullptr;;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Base &rhs);
private:
    char *_pbase;
};

std::ostream &operator<<(std::ostream &os, const Base &rhs)
{
    if(rhs._pbase)
    {
        os << rhs._pbase;
    }
    return os;
}

class Derived
: public Base
{
public:
    Derived(const char *pbase)
    : Base(pbase)
    {
        cout << "Derived(const char *)" << endl;
    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
    }
    friend std::ostream &operator<<(std::ostream &os, const Derived &rhs);

};

std::ostream &operator<<(std::ostream &os, const Derived &rhs)
{
    const Base &ref = rhs;
    os << ref;

    return os;
}
int main(int argc, char **argv)
{
    Derived d1("hello");
    cout << "d1 = " << d1 << endl;

    cout <<endl << endl;
    Derived d2 = d1;
    cout << "d1 = " << d1 << endl;
    cout << "d2 = " << d2 << endl;

    cout <<endl << endl;
    Derived d3("world");
    cout << "d3 = " << d3 << endl;

    cout << endl << endl;
    d3 = d1;
    cout << "d1 = " << d1 << endl;
    cout << "d3 = " << d3 << endl;
    return 0;
}

