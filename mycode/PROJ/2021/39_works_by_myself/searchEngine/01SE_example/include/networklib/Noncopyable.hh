/* <Noncopyable> -*- C++ -*- */
#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace wd
{

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}

    Noncopyable(const Noncopyable &) = delete;
    Noncopyable & operator=(const Noncopyable &) = delete;
};

}// end of namespace wd
#endif

