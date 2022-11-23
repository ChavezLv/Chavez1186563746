#include <iostream>

using std::cout;
using std::endl;

//复数
class Complex
{
public:
    Complex(double dreal = 0, double dimage = 0)
    : _dreal(dreal)
    , _dimage(dimage)
    {
        cout << "Complex(double = 0, double = 0)" << endl;
    }

    double getReal() const
    {
        return _dreal;
    }

    double getImage() const
    {
        return _dimage;
    }

    /* Complex operator+(const Complex &lhs, const Complex &rhs) */
    //非静态的成员函数的第一个参数的位置隐藏了一个this指针
    //2、运算符重载之成员函数的形式
    Complex operator+(const Complex &rhs)
    {
        cout << "Complex operator+(const Complex &)" << endl;
        return Complex(_dreal + rhs._dreal , _dimage + rhs._dimage);
    }

    void print() const
    {
        cout << _dreal << " + " << _dimage << "i" << endl;
    }

    ~Complex()
    {
        cout << "~Complex()" << endl;
    }
private:
    double _dreal;
    double _dimage;
};
#if 0
int operator+(int x, int y)
{

}

Z == X + Y
#endif

void test()
{
    Complex c1(1, 2);
    cout << "c1 = ";
    c1.print();

    cout << endl;
    Complex c2(3, 4);
    cout << "c2 = ";
    c2.print();

    cout << endl <<endl;
    Complex c3 = c1 + c2;
    cout << "c3 = ";
    c3.print();
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

