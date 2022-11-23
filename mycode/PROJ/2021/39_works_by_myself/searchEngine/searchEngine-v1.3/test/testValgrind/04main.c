/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：04main.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 12:00:54
*   @description：
*
================================================================*/


#include <stdlib.h>
#include <unistd.h>
int main( void )
{
	char* arr  = (char*)malloc(10);
	free(arr);
	free(arr);//Invalid free() / delete / delete[] / realloc()
	return 0;
}

