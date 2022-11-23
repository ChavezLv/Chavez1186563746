/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：05main.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 12:05:35
*   @description：
*
================================================================*/


#include <stdlib.h>
#include <unistd.h>
int main( void )
{
	char* arr  = (char*)malloc(10);
	delete arr;//Mismatched free() / delete / delete []
	return 0;
}

