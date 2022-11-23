/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：test.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 20:54:50
*   @description：
*
================================================================*/

#include <iostream>
using std::endl;
using std::cout;

class A{
public:

    A(){
        cout << "A()" << endl;
    }
    A(const A &lhs){
        cout << "A(const A & lhs)" << endl;
    }
    A& operator=(const A&lhs){
        cout << "A & operator=(const A &lhs)" << endl;
        return *this;
    }
};

int& test(){
    int* i = new(int);
    *i = 10;
    return *i;
}
A& test1(){
    A* i = new A();
    return *i;
}
void test01(){
    int &i = test();
    std::cout << i << std::endl;
    A &j = test1();
    /* A &j = test1(); */
}
int main(){

    /* test01(); */
    A i;
    A & b  = i;
    A c = b;//拷贝构造
    /* A & c = b; */
    return 0;
}

