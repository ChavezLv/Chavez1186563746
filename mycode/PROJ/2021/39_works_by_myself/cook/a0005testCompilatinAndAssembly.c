/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0005testCompilatinAndAssembly.c
*   @author     ：anonymous
*   @date       ：2021年09月11日 22:16:39
*   @description：
*
================================================================*/


#include <stdio.h>
int main()
{
    char   a   =   1;
    char   c[]   =   "1234567890";
    char   *p   = "1234567890";
    a   =   c[0];
    a   =   p[10];// 第一种在读取时直接就把字符串中的元素读到寄存器cl中，
    //而第二种则要先把指针值读到  edx中，再根据edx读取字符，显然慢了。
    return 0;
}
