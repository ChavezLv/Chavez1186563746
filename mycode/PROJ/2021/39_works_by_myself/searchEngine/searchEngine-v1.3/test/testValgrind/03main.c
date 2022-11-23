/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：03main.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 11:55:57
*   @description：
*
================================================================*/

/*
 *Syscall param * uninitialised byte(s)
 * 在系统调用中使用未初始化或不可寻址的值。
 * */

#include <stdlib.h>
#include <unistd.h>
int main( void )
{
    char* arr  = (char*)malloc(10);//uninitialised byte(s)
    int*  arr2 = (int*)malloc(sizeof(int));
    write( 1 /* stdout */, arr, 10 );
    exit(arr2[0]);
}

