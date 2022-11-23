/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0012testTemplate.c
*   @author     ：anonymous
*   @date       ：2021年09月14日 21:02:22
*   @description：
*
================================================================*/



#ifdef __TEMP_CC__

/* #ifndef __TEMP_CC__ */
/* #define __TEMP_CC__ */
#include "a0012testTemplate.h" //方法二:

template<typename T1>
T1 Temp<T1>:: add(T1 x, T1 y){
    return x + y;

}

/* template class Temp<int>;//方法三 啥也不干就多一个这个*/

#endif
