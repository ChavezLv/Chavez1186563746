/**
 * Project netli_v1.0
 */


#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace wd{


class Noncopyable {
public: 
    
protected: 
    Noncopyable(){  }
    ~Noncopyable() {  }

    Noncopyable(const Noncopyable &) = delete ;
    Noncopyable & operator= (const Noncopyable &) = delete;
};

}//end of namespace wd

#endif //_NONCOPYABLE_H
