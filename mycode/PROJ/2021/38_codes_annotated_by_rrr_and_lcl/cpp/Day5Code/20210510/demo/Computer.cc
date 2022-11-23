#include "Computer.h"
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//静态数据成员要放在全局静态位置进行初始化，特别是对于头文件与
//实现文件的形式，要放在实现文件里面，否则会有多重定义的报错

float Computer::_totalPrice = 0.0;

Computer::Computer(const char *brand, float price)
: _brand(new char[strlen(brand) + 1]())
, _price(price)
/* , _totalPrice(0)//error */
{
    /* _brand = new char[strlen(brand) + 1](); */
    cout << "Computer(const char *, float)" << endl;
    strcpy(_brand, brand);
    _totalPrice += _price;
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

//静态的成员函数的第一个参数位置没有隐含的this指针
//静态的成员函数不能访问非静态的数据成员和非静态的成员函数
//非静态的成员函数可以访问静态的数据成员和静态的成员函数
void Computer::printTotalPrice()
{
    /* this->_price = 100;//error */
    /* print();//error */
    cout << "totalPrice = " << _totalPrice << endl;
}

void Computer::print(/*  Computer * const this  */)
{
    /* this->_price = 10; */
    /* printTotalPrice();//ok */
    cout << "void print()" << endl;
    printf("brand : %p\n", _brand);
    cout << "brand : " << _brand << endl
         << "price : " << this->_price << endl;
}

void Computer::print(/* const Computer * const this */) const
{
    /* this->_price = 10;//error */
    cout << "void print() const " << endl;
    printf("brand : %p\n", _brand);
    cout << "brand : " << _brand << endl
         << "price : " << this->_price << endl;
}

void func(const Computer &rhs)
{
    rhs.print();
}
Computer::~Computer()
{
    cout << "~Computer()" << endl;
    _totalPrice -= _price;
    /* if(_brand != nullptr) */
    if(_brand)
    {
        /* cout << "delete [] _brand" << endl; */
        delete [] _brand;
        _brand = nullptr;
    }
}
