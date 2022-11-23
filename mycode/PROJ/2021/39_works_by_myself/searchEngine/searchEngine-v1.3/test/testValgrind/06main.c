/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：06main.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 12:07:26
*   @description：
*
================================================================*/


#include<string.h>

int main( void )
{
	char a[12] = {'h','e','l', 'l','o','\0'};
	memcpy(a+3,a,6);//Source and destination overlap in memcpy
    /* memcpy(a, a, 6); //why no warning when use valgrind?*/
	return 0;
}

