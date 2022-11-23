#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

struct Computer
{
    //类的内部，左大括号到右大括号称为类的内部
public:
    //成员函数
    void setBrand(const char *brand)
    {
        strcpy(_brand, brand);//先不去考虑越界的问题
    }

    void setPrice(float price)
    {
        _price = price;
    }

    void print()
    {
        cout << "brand : " << _brand << endl
             << "price : " << _price << endl;
    }
private://封装性
    //数据成员
    char _brand[20];
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

    com._price = 3000;//error, 封装性的特点
    return 0;
}

