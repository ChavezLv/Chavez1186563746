#include "Computer.h"
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

Computer::Computer(const char *brand, float price)
: _brand(new char[strlen(brand) + 1]())//构造函数
, _price(price)
{
    /* _brand = new char[strlen(brand) + 1](); */
    cout << "Computer(const char *, float)" << endl; //
    strcpy(_brand, brand); //
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
