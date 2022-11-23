/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0004testTwoDimensionalArray.c
*   @author     ：anonymous
*   @date       ：2021年09月11日 21:50:45
*   @description：
*
================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void test01(){
    //c
    int (*p)[6] = (int(*)[6]) malloc(3 * sizeof(int) * 6);
    /* p[0] = {0, 0, 0, 0, 0, 1};//error */

    //c++
    /* int (*pNew)[6] = new int[6 * 3]();//error */
    /* int **pNew = new int[18]();//error */
    int *pNew = new int[18]();//ok
}
void test02(){
    int **pp = (int**) malloc(3 * sizeof(int*));
    for(size_t i = 0; i < 3; ++i){
        pp[i] = (int*) malloc(6 * sizeof(int));
    }

    pp[0][0] = 0;
}
int main(){
    test01();

    return 0;
}
