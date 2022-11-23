#ifndef __ADD_H__
#define __ADD_H__

//模板不能分成头文件与实现文件的形式，在声明的时候必须要看到实现
//
template <class T>
T add(T x, T y);

#include "add.cc"
//course1 
// 笔记by lcl:\
1)错误: 比如 此时 test.cc中推导为int add(int x,int y);\
看不到实现!就会出错\
\
类比内联函数inline\
\
这个可以看下c++/7/中的源码也是 basic_string.tcc包含在了头文件里面;\
\
2)对于模板而言, 声明必有实现\
c++头文件都是用模板写的, 所以编译时必须看到实现



#endif
