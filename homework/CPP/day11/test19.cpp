#include<iostream>
using namespace std;
class A
{ 
public:
    virtual
    void func(int val = 1)
    {
        cout << "A->" << val << endl;
    }
    void test()
    {
        func();
    }
private:
    long _a;
};

class B
: public A
{ 
public:
    virtual
    void func(int val = 10)
    {
        cout << "B->" << val << endl;
    }
    long _b;
};

class C
: public B
{
    virtual
    void func(int val = 10)
    {
        cout << "C->" << val << endl;
    }   
    
};

int main(void)
{
    B b;
    A *p3=&b;
    //error:p3->_b;
    p3->func();
#if 0
    B b;
    A *p1 = (A*)&b;
    cout<<"before cast:"<<p1<<endl;
    B *p2 = &b;
    cout<<"after cast:"<<&b<<endl;
    //对象切片p1->_b;
    p1->func();
    //p1->B::func();


    //p1->B::func();
    p2->A::func();
    p2->_b;
    p2->func();
    A *p3;
    p3=&b;
    p3->func();//不应该调用B::func?
    //p3->_b;
    C c;
#endif
    return 0;
}
