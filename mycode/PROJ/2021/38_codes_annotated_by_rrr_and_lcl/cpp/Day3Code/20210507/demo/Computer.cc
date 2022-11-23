#include "Computer.h"
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

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
    cout << "brand : " << _brand << endl
         << "price : " << _price << endl;
}
