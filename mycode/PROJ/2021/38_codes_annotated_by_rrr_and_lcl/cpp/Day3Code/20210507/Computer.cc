#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//C++中对struct的功能做了提升，不仅可以定义数据也可以定义函数，
//然后struct与class的唯一区别就是：struct的默认访问权限是public
//然后class默认访问权限是private
class Computer
{
    //类的内部，左大括号到右大括号称为类的内部
public:
    //成员函数
    inline
    void setBrand(const char *brand)
    {
        strcpy(_brand, brand);//先不去考虑越界的问题
    }

    inline
    void setPrice(float price)
    {
        _price = price;
    }

    inline
    void print()
    {
        cout << "brand : " << _brand << endl
             << "price : " << _price << endl;
    }
private://封装性
    //数据成员
    char _brand[20];//m_brand   brand_
    float _price;
};

int main(int argc, char **argv)
{
    //可以吧Computer看成一种类型，类型称为自定义类型
    //int a;
    //对象的创建
    Computer com;
    com.setBrand("huawei");
    com.setPrice(6000);
    com.print();
    /* Computer com2; */

    /* com._price = 3000;//error, 封装性的特点 */
    return 0;
}

