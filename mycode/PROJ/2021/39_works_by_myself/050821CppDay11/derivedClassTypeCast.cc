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
    A(int a)
    : _a(a){

    }
private:
    int _a;
};
class B {
public:
    B(int b)
    : _b(b){

    }
    void show(){
        cout << _b << endl;
    }
private:
    int _b;
};
class C 
: public A
, public B{
public:
    C(int a, int b, int c)
    : A(a)
    , B(b)
    , _c(c){

    }
private:
    int _c;
};

class D
: public B{
public:
    D(int b = 0)
    : B(b){

    }
private:
    /* int _d; */
};
int main()
{
    C c(1, 2, 3);
    /* B b(c); */
    B b(3);
    b.show();

    b = c;//向上转型时, 从内存角度, 会找到向上兼容的类型的对应位置, b 不会 取到a位置的值
    b.show();

    D d;
    /* d = c;//error, 非派生类的类型转换需要重载才不会出错 */
    return 0;
}

