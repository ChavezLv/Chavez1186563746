#ifndef __ADD_H__
#define __ADD_H__

//对于内联函数（inline函数）而言，不要分成头文件与实现文件的形式
//不能将声明与实现分开
//可以使用#include进行包含实现文件

//内联函数一般都是一些小函数，不要去写for/while这些复杂的语句
inline
int add(int x, int y);

#include "add.cc"

#endif
