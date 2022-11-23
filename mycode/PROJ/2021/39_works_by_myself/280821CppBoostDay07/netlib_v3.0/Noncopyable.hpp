/*================================================================
*   
*   @file       ：Noncopyable.hpp
*   @date       ：2021年08月29日 20:32:45
*   @description：
*
================================================================*/


#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__

namespace wd{

class Noncopyable{
public:
    Noncopyable() {  }//显示写出来的目的就是继承的不用在初始化列表里写
    ~Noncopyable() {  }
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator= (const Noncopyable &) = delete;
    

};

}//end of namespace wd

#endif //NONCOPYABLE_HPP
