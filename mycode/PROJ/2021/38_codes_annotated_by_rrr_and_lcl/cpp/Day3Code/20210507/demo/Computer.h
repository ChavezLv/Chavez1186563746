#ifndef __COMPUTER_H__
#define __COMPUTER_H__

class Computer
{
public:
    //成员函数
    //其实也可以公共数据成员，只是一般都不会这么做
    void setBrand(const char *brand);
    void setPrice(float price);
    void print();

private://封装性
    //数据成员
    char _brand[20];//m_brand   brand_
    float _price; //价钱
};

#endif
