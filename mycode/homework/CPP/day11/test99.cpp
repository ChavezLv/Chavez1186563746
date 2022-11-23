#include<iostream>
using namespace std;
class Base
{
public:
virtual void display()
{
cout << "Base::display()" << endl;
}
virtual void print()
{
cout << "Base::print()" << endl;
}
};
class Derived
: public Base
{
public:
virtual void display()
{
cout << "Derived::display()" << endl;
}
};
void test(Base *pbase)
{
pbase->display();
}
int main()
{
Base base;
Derived derived;
test(&base);
test(&derived);
return 0;
}

