/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0012testTemplate.h
*   @author     ：anonymous
*   @date       ：2021年09月14日 21:04:25
*   @description：
*
================================================================*/


#ifndef __TEMP_H__
#define __TEMP_H__


template<typename T1>
class Temp{
public:
    Temp(){}
    ~Temp(){}

    T1 add(T1 x, T1 y);

};

#define __TEMP_CC__
#include "a0012testTemplate.c"
#endif

