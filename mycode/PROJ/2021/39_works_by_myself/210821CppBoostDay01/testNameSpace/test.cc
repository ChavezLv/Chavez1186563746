#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <algorithm>

class A {
public:
   /* virtual void fun() = 0; */
    virtual void fun() {
        cout << "fun() " << endl;
    }
};

class B
: public A
{
public:
    //方法后加 override，可以方便程序员阅读代码，知道这方法是重写的，不用回去找
    void fun()  override;

};
void B::fun() {
    cout << "B::func()" << endl;
}
void test01(){
    B b;
    b.fun();
    
}
int main(int argc, char* argv[])
{
   
       test01();
          return 0;
}
/*================================================================
*   Copyright (C) 2021 ゞゞゞ All rights reserved.
*   
*   @file       ：test.cc
*   @author     ：ゞゞゞ
*   @date       ：2021年08月26日 22:53:30
*   @description：
*
================================================================*/


