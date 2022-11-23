/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：test.c
*   @author     ：anonymous
*   @date       ：2021年09月11日 17:11:00
*   @description：
*
================================================================*/


#include <stdio.h>
#include <iostream>
using namespace std;

void test01(){

    const char p[] = "hello";
    printf("%p\n", p);

    /* char * pb = &p[0];//error */
    const char * pc = &p[0];
    pc = "j"; 

    cerr << pc <<endl;


    const char *p1 = "hello";
    const char * pc1 = &p1[0];
    /* *pc1 = 'a';//error */
    
    printf("%p\n", p1);

}
int main(){
    test01();

    return 0;
}
