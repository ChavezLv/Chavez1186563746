#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class Computer
{
public:
    //成员函数
    void setBrand(const char *brand);
    void setPrice(float price);
    void print();

private://封装性
    //数据成员
    char _brand[20];//m_brand   brand_
    float _price;
};

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

int main(int argc, char **argv)
{
    //对象的创建
    Computer com;
    com.setBrand("huawei");
    com.setPrice(6000);
    com.print();
    /* Computer com2; */

    /* com._price = 3000;//error, 封装性的特点 */
    return 0;
}

