#ifndef __COMPUTER_H__
#define __COMPUTER_H__

class Computer
{
public:
    Computer(const char *brand, float price);
    Computer(const Computer &rhs);
    Computer &operator=(const Computer &rhs);
    void setBrand(const char *brand);
    void setPrice(float price);
    static void printTotalPrice();

    //const版本的成员函数可以与非const版本的成员函数进行重载（this指针是不一样）
    //一般建议先写出const版本的成员函数
    void print();
    void print() const;
    ~Computer();

private:
    char *_brand;//8
    float _price;//4, 内存对齐
    static float _totalPrice;//4静态的数据成员不占类的大小，被类
                            //创建的所有对象所共享
};

#endif
