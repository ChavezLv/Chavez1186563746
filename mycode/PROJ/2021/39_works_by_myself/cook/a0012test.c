/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0012test.c
*   @author     ：anonymous
*   @date       ：2021年09月14日 21:07:51
*   @description：
*
================================================================*/

#include "a0012testTemplate.h"//方法一
/* #include "a0012testTemplate.c"//方法二 */
#include <iostream>

using namespace std;

void test01(){
    Temp<int> test;
    cout << test.add(5, 6) << endl;
}

int main(){
    test01();
    return 0;
}

