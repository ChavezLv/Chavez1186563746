#include<iostream>
using namespace std;

struct Foo{
    //必须是public
    //获取成员函数地址，必须用去地址符号&
    void print(){
        cout<<"foo::printf"<<endl;
    }
    void display(){
        cout<<"foo::display"<<endl;
    }
};
typedef  void (Foo::*MemberFunction)();
int main(){
    MemberFunction f0=&Foo::print;
    MemberFunction f1=&Foo::display;
    //f1();
    Foo foo;
    (foo.*f0)();//.* 成员指针访问运算符；
    Foo *fp=nullptr;
    (fp->*f0)();
    return 0;
}
