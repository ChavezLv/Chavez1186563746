#ifndef __WD_NONCOPYABLE_HPP__ 
#define __WD_NONCOPYABLE_HPP__


namespace wd{

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}

    Noncopyable(const Noncopyable&)=delete;
    Noncopyable &operator=(const Noncopyable&)=delete; 
};
}//end of namespace wd

#endif
