#include <iostream>

using std::cout;
using std::endl;


class A
{
public:
    virtual
    void a()
    {
        cout << "A::a()" << endl;
    }

    virtual
    void b()
    {
        cout << "A::b()" << endl;
    }

    virtual
    void c()
    {
        cout << "A::c()" << endl;
    }
};

class B
{
public:
    virtual
    void a()
    {
        cout << "B::a()" << endl;
    }

    virtual
    void b()
    {
        cout << "B::b()" << endl;
    }

    void c()
    {
        cout << "B::c()" << endl;
    }

    void d()
    {
        cout << "B::d()" << endl;
    }
};

class C
: public A
, public B
{
public:
    virtual
    void a()
    {
        cout << "C::a()" << endl;
    }

    void c()
    {
        cout << "C::c()" << endl;
    }

    void d()
    {
        cout << "C::d()" << endl;
    }
};

class D
: public C
{
public:
    void c()
    {
        cout << "D::c()" << endl;
    }
};

void test()
{
    cout << "sizeof(A) = " << sizeof(A) << endl;
    cout << "sizeof(B) = " << sizeof(B) << endl;
    cout << "sizeof(C) = " << sizeof(C) << endl << endl;

    C c;
    printf("&c = %p\n", &c);

    cout << endl;
    A *pa = &c;
    printf("pa = %p\n", pa);
    pa->a();
    pa->b();
    pa->c();

    cout << endl;
    B *pb = &c;
    printf("pc = %p\n", pb);
    pb->a();
    pb->b();
    pb->c();
    pb->d();

    cout << endl;
    C *pc = &c;
    printf("pc = %p\n", pc);
    pc->a();
    /* pc->b();//error,二义性 */
    pc->A::b();
    pc->B::b();
    pc->c();
    pc->d();//隐藏

    cout << endl;
    D d;
    pc = &d;
    pc->c();
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

