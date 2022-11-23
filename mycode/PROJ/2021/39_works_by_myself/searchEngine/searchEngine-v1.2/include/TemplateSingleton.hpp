/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：TemplateSingleton.hpp
*   @author     ：anonymous
*   @date       ：2021年09月02日 16:05:23
*   @description：
*
================================================================*/


#ifndef __WD_TEMPLATESINGLETON_HPP__
#define __WD_TEMPLATESINGLETON_HPP__

#include <iostream>
using std::cout;
using std::endl;

namespace wd{


template<typename _Tp>
class Singleton {
private:
    //嵌套类一般私有
    class AutoRelease{
    public:
        AutoRelease(){
            /* cout << "AutoRelease()" << endl; */
        }
        ~AutoRelease(){
            /* cout << "~AutoRelease()" << endl; */
            if(_pInstance){
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
public:
    virtual
        void print() const = 0;
    template<typename ... Args> 
        static _Tp *getInstance(Args ... args){//当用基类指针指向这个new的堆...
            /* pthread_once(&_once, init(args ...)); // error*/
            if(!_pInstance){
                _pInstance = new _Tp(args ...);
                /* atexit(rmInstance);//自动析构 */
                _auto;
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

    //虚析构: 当getInstance返回的值改为基类指针时就有用
    //比如: static Singleton<_Tp> * _pInstance;
    //比如: static Singleton<_Tp> *getInstance(Args ... args){}
    virtual
        ~Singleton(){
            cout << "~Singleton()" << endl;

        };
private:
    static _Tp * _pInstance; 
    static AutoRelease _auto;
};

template<typename _Tp>
_Tp* Singleton<_Tp>::_pInstance = nullptr;

template<typename _Tp>
typename Singleton<_Tp>::AutoRelease Singleton<_Tp>::_auto;

#if 0

class Computer//私有构造
/* : public Singleton<Computer> */
{
private:
    friend Singleton<Computer>;//ok     私有构造
    Computer(std::string brand = "", double price = 0)
        : _brand(brand), _price(price){  }
    ~Computer(){ cout << "~Computer()" << endl;  }
public:    
    void print() const{
        cout << "brand(" << _brand << "), price(" << _price << ")" << endl;

    }
private:
    std::string _brand;
    double _price;

};
//非继承类
void test(){
    Computer *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    pc1->print();
    cout << pc1 << endl;
    /* Singleton<Computer> *pc3 = Singleton<Computer>::getInstance("Xiaomi", 6666); //error*/

}

#endif

}//end of namespace wd

#endif //__WD_TEMPLATESINGLETON_HPP__
