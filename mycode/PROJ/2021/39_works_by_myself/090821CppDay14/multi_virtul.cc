#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class A {
public:
    virtual void  a(){ cout << "A:a" << endl; }
    virtual void  b(){ cout << "A:b" << endl; }
    virtual void  c(){ cout << "A:c" << endl; }
    void d(){ cout << "A:d" << endl; }

};
class B { public:
    virtual void  a(){ cout << "B:a" << endl; }
    virtual void  b(){ cout << "B:b" << endl; }
    void  c(){ cout << "B:c" << endl; }
    void d(){ cout << "B:d" << endl; }

};
class C : public A, public B{ public:
    void  a(){ cout << "C:a" << endl; }
    void  b(){ cout << "C:b" << endl; }
    void  c(){ cout << "C:c" << endl; }
    void d(){ cout << "C:d" << endl; }

};
int main()
{
    C c; 
    B* pb = &c;
    pb->a();
    pb->b();
    pb->c();

    cout << endl;

    A* pa = &c;
    pa->a();
    pa->b();
    pa->c();

    cout << endl;

    C* pc = &c;
    pc->a();
    pc->b();

    pc->c();
    pc->A::c();
    pc->B::c();
    
    pc->d();


    return 0;
}

