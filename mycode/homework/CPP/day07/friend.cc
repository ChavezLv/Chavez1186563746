#include<iostream>
using namespace std;
void test(int){
//test可以看成是对象
//每一个函数名都可以看成是相应类型的对象
//函数对象作为STL六大组件之一，可以灵活运用
    static //静态变量离开函数后不可访问
}
//class也一样
struct FunctionObject{
    //函数调用运算符重载；
    int operator()(){
        cout<<"overload function"<<endl;
        return 1;    }
    int operator()(int x,int y){
        cout<<"overload"<<endl;
        return x+y;
    }
    int _cnt;
    //函数对象相比普通函数而言，携带了状态
    //如记录函数调用次数；
};

int main(){
    //C++中，面向对象，都可以看成对象；
    FunctionObject fo;
    cout<<fo()<<endl;//泛型编程,对象的函数类比用法，称为函数对象
                     //实际上：fo.operator()();
    cout<<fo(3,5)<<endl;//泛型编程
}
