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

//单个虚继承，带虚函数
class A {
public:
    virtual
    void f(){ cout << "A::f()" << endl; }
private:
    int _a;
};
class B
: 
    /* virtual */ 
    public A
{
public:
    /* void f(){} */
private:
    int _b;
};
int main()
{
    B b;
    A a;
    printf("B f %p\n", (long*)*(long*)&b);//*(long*)&b)就是虚表的开头
    printf("A f %p\n", (long*)*(long*)&a);

    typedef void (*pF)();
    /* pF f = (pF)(*(long*)*(long*)&b); */
    pF f = (pF)*(long*)*(long*)&b;
    (pF)*(long*)*(long*)&b;
    f();
    printf("B f %p\n", (long*)*(long*)*(long*)&b);
    printf("A f %p\n", (long*)*(long*)*(long*)&a);
    return 0;
}

