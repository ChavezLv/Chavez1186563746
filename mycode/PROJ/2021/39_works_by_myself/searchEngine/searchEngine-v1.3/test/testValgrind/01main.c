/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：01main.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 11:33:06
*   @description：
*
================================================================*/



#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *x = (int *)malloc(sizeof(int)*10);
	int i;
	for(i=0; i<=10; ++i)
	{
		x[i] = i;//当i=10时，越界，非法访问
	}
}


