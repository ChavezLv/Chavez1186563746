/*================================================================
*   Copyright (C) 2021 WD39th-某某某 All rights reserved.
*   
*   @file       ：Noncopyable.hh
*   @author     ：WD39th-某某某
*   @date       ：2021年08月23日 19:59:04
*   @description：
*
================================================================*/


#ifndef __WD_NONCOPYABLE_HPP__
#define __WD_NONCOPYABLE_HPP__

namespace wd
{

class Noncopyable{
protected://定义了protected构造函数的类也是一个抽象类
    Noncopyable() {  }
    ~Noncopyable() {  }
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable & operator=(const Noncopyable &) = delete;

};



}//end of namespace wd


#endif //NONCOPYABLE_HH
