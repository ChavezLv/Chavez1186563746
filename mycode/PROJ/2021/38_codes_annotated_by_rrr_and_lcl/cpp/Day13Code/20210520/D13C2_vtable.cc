#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    Base(long base = 0)
    : _base(base)
    {
        cout << "Base(long = 0)" << endl;
    }

    virtual
    void f()
    {
        cout << "virtual void Base::f()" << endl;
    }

    virtual
    void g()
    {
        cout << "virtual void Base::g()" << endl;
    }

    virtual
    void h()
    {
        cout << "virtual void Base::h()" << endl;
    }

    ~Base()
    {
        cout << "~Base()" << endl;
    }
private:
    long _base;
};

class Derived
: public Base
{
public:
    Derived(long base = 0, long derived = 0)
    : Base(base)
    , _derived(derived)
    {
        cout << "Derived(long = 0)" << endl;
    }

    virtual
    void f()
    {
        cout << "virtual void Derived::f()" << endl;
    }

    virtual
    void g()
    {
        cout << "virtual void Derived::g()" << endl;
    }

    virtual
    void h()
    {
        cout << "virtual void Derived::h()" << endl;
    }

    ~Derived()
    {
        cout << "~Derived()" << endl;
    }
private:
    long _derived;
};

void test0(){
    Derived d(10, 100);
    long *pvtalble = (long*)&d;//一级指针指向对象的内存布局
    //此时用pvtable[0](虚函数指针) [1](base的成员数据) [2](derived的成员数据
    //) 依次访问,就可以直接操作内存破解封装性

    long *pvtable2 = (long*)pvtable2[0];//二级指针指向对象内存的虚基指针
    //这是直接用(long*)指向虚基指针的值, 之后再通过[]偏移就是各个虚函数的入口地址

    // 函数类型 :返回值 参数列表
    typedef void (*Function)();
    Function f1 = (Function)pvtable2[0];
    f1();//如果便可以一次执行这些虚函数, 但是会有问题? 因为this 指针没赋值!

    typedef void (*Function2)(Derived*);
    Function f2 = (Function2)pvtable2[0];
    f2(&d);

}
int main(int argc, char **argv)
{
    //虚表是存在的，虚表存在于只读段,对于普通的单继承而言，一个类
    //只有一张虚表
    Derived derived(10, 20);
    printf("对象derived的地址 : %p\n", &derived);
    printf("对象derived的地址 : %p\n", (long *)&derived);
    /* printf("虚表的地址 : %p\n", *(long *)&derived);//ok */
    printf("虚表的地址 : %p\n", (long *)*(long *)&derived);
    printf("虚函数的地址 : %p\n", (long *)*(long *)*(long *)&derived);

    cout << endl << endl;
    typedef void (*Function)();
    Function pf;
    pf =  (Function)*((long *)*(long *)&derived);
    pf();
    printf("第一个虚函数的地址 : %p\n", pf);

    cout << endl;
    pf =  (Function)*((long *)*(long *)&derived + 1);
    pf();
    printf("第二个虚函数的地址 : %p\n", pf);

    cout << endl;
    pf =  (Function)*((long *)*(long *)&derived + 2);
    pf();
    printf("第三个虚函数的地址 : %p\n", pf);


    cout << endl;
    Derived derived2(100, 200);
    printf("对象derived2的地址 : %p\n", &derived2);
    printf("对象derived2的地址 : %p\n", (long *)&derived2);
    /* printf("虚表的地址 : %p\n", *(long *)&derived2);//ok */
    printf("虚表的地址 : %p\n", (long *)*(long *)&derived2);
    printf("虚函数的地址 : %p\n", (long *)*(long *)*(long *)&derived2);

    cout << endl;
    cout << "打印数据成员_base = " 
        <<   (long)*((long *)&derived + 1) << endl;

    cout << "打印数据成员_derived = " 
        <<   (long)*((long *)&derived + 2) << endl;


    return 0;
}

