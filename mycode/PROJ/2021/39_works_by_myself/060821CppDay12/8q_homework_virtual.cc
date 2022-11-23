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
    virtual void func(int val = 1){
        cout << "A:" << val << endl;
    }
    /* virtual void test(){ */
    /*     func(); */
    /* } */
};
class B
: public A{
public:
    virtual void func(int val = 10){
        cout << "B:" << val << endl;
    }
};
int main()
{
    B b;
    A *p1 = (A*) &b;
    A *p2 = new B();
    B *p3 = &b;
    p1->func(1);//静态绑定
    p2->func(1);
    p3->func(10);

    return 0;
}

