#include "Computer.h"
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

Computer::Computer(const char *brand, float price)
: _brand(new char[strlen(brand) + 1]())
, _price(price)
{
    /* _brand = new char[strlen(brand) + 1](); */
    cout << "Computer(const char *, float)" << endl;
    strcpy(_brand, brand);
}

#if 0
Computer::Computer(const Computer &rhs)
: _brand(rhs._brand)//浅拷贝
, _price(rhs._price)
{
    cout << "Computer(const Computer &)" << endl;
}
#endif

#if 1
//问题1：拷贝构造函数参数中的引用符号可以去掉吗？
//
//问题2：拷贝构造函数参数中的const可以去掉吗？
//
//
Computer::Computer(const Computer &rhs)
: _brand(new char[strlen(rhs._brand) + 1]())//深拷贝
, _price(rhs._price)
{
    cout << "Computer(const Computer &)" << endl;
    strcpy(_brand, rhs._brand);
}
#endif
#if 0
//默认赋值运算符不够用
Computer& Computer::operator=(const Computer &rhs)
{
    cout << "Computer &operator=(const Computer &)" << endl;
    _brand = rhs._brand;
    _price = rhs._price;

    return *this;
}
#endif

//com2 = com1
Computer& Computer::operator=(const Computer &rhs)
{
    cout << "Computer &operator=(const Computer &)" << endl;
    //if (*this != rhs)
    if(this != &rhs)//1、自复制(考虑自己赋值给自己)
    {
        delete [] _brand;//2、释放左操作数(不释放就会有内存泄漏)
        _brand = nullptr;

        _brand = new char[strlen(rhs._brand) + 1]();//3、深拷贝
        strcpy(_brand, rhs._brand);

        _price = rhs._price;

    }

    return *this;//4、返回*this
}

void Computer::setBrand(const char *brand)
{
    strcpy(_brand, brand);//先不去考虑越界的问题
}

void Computer::setPrice(float price)
{
    _price = price;
}

void Computer::print()
{
    printf("brand : %p\n", _brand);
    cout << "brand : " << _brand << endl
         << "price : " << _price << endl;
}

Computer::~Computer()
{
    cout << "~Computer()" << endl;
    /* if(_brand != nullptr) */
    if(_brand)
    {
        /* cout << "delete [] _brand" << endl; */
        delete [] _brand;
        _brand = nullptr;
    }
}
