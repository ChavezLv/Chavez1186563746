#ifndef __TEMPLATESINGLETON_HH__
#define __TEMPLATESINGLETON_HH__
#include <iostream>
#include <iostream>

using std::cout;
using std::endl;

template<typename _Tp>
class Singleton {
public:
    virtual
        void print() const = 0;
    template<typename ... Args> 
        static _Tp *getInstance(Args ... args){//当用基类指针指向这个new的堆...
            /* pthread_once(&_once, init(args ...)); // error*/
            if(!_pInstance){
                _pInstance = new _Tp(args ...);
                atexit(rmInstance);//自动析构
            }
            return _pInstance;
        }
private:
    static void  rmInstance(){
        if(_pInstance){
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

protected:
    Singleton() = default;//防止实例
    Singleton(const Singleton&) = delete ;
    Singleton & operator=(const Singleton&) = delete ;

    //虚析构: 当getInstance返回的是基类指针时就有用
    //比如: static Singleton<_Tp> * _pInstance;
    //比如: static Singleton<_Tp> *getInstance(Args ... args){}
    virtual
        ~Singleton(){
            cout << "~Singleton()" << endl;

        };
private:
    static _Tp * _pInstance; 
};

template<typename _Tp>
_Tp* Singleton<_Tp>::_pInstance = nullptr;


#endif
