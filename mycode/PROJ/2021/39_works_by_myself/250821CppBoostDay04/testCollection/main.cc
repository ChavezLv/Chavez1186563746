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
#include <functional>
using std::function;

class Solution {
public:
    Solution(int data)
    : _data(10){

    }
    int _data;
   
};

using ThreadCallback = function<void()>;
void fun1(){
    cout << "func" << endl;
}
//传入的形参后面加&&，变成万能引用
/* void call(ThreadCallback && f){ */
void call(ThreadCallback  f){
    f();
}
void test01(){
    Solution sol(10);
    function<int()> f1 = std::bind(&Solution::_data, &sol);
    /* int f1 = std::bind(&Solution::_data, &sol); */
    cout << f1() << endl;
    
    int i = 10;
    /* auto f = std::bind(&i); */
    /* cout << "&f" << &f << endl;; */



    ThreadCallback f2 = std::bind(fun1);
    f2();
    call(f2);
    call(fun1);
    call(&fun1);
    call(*fun1);

}
void test02(){
    /* int &a = 10; // error*/
    const int &b = 10;


}
int main(int argc, char* argv[])
{
   
       test01();
          return 0;
}
/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：main.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月25日 20:32:59
*   @description：
*
================================================================*/


