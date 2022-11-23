/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：07main.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 12:12:09
*   @description：
*
================================================================*/


#include <stdlib.h>
#include <unistd.h>
int main( void )
{
    /*
     * 所有内存分配函数都使用一个参数来指定应分配的内存块的大小。
     * 显然，请求的大小应该是非负值，并且通常不会过大。
     * 例如，在64位计算机上，分配请求的大小超过2 ** 63字节或者是负值。
     * 这样的值被称为“可疑的值”。*/
	char* arr  = (char *)malloc(-1);
    // Argument 'size' of function malloc has a fishy (possibly negative) value: -1
	free(arr);
	return 0;
}

