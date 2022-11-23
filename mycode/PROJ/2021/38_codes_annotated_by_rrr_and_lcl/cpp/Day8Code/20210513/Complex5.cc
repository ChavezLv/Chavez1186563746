#include <iostream>
#include <limits>

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

    //c1 += c2;
    //对于复合赋值运算符而言，推荐使用成员函数的形式
    Complex &operator+=(const Complex &rhs)
    {
        cout << "Complex operator+=(const Complex &)" << endl;
        _dreal += rhs._dreal;
        _dimage += rhs._dimage;

        return *this;
    }

    //前置++与后置++的区别？
    //前置++比后置++快，因为前置++没有返回局部变量，没有执行拷贝构造函数
    //或者说少执行了几次函数，而函数执行是有开销的
    //
    //前置++返回值是左值，后置++返回值是右值
    Complex &operator++()
    {
        cout << "Complex &operator++()" << endl;
        ++_dreal;
        ++_dimage;

        return *this;
    }

    Complex operator++(int)//int只是起一个标志作用
    {
        cout << "Complex operator++(int)" << endl;
        Complex tmp(*this);

        _dreal++;
        _dimage++;

        return tmp;
        
    }

    void print() const
    {
        /* cout << _dreal << " + " << _dimage << "i" << endl; */
        if(0 == _dreal && 0 == _dimage)
        {
            cout << 0 << endl;
        }
        else if(0 == _dreal)
        {
            cout << _dimage << "i" << endl;
        }
        else
        {
            //1 + i
            cout << _dreal;
            if(_dimage > 0)
            {
                cout << " + " << _dimage << "i" << endl;
            }
            else if(_dimage < 0)
            {
                cout << " - " << (-1)*_dimage << "i" << endl;
            }
            else
            {
                cout << endl;
            }
        }
    }

    ~Complex()
    {
        cout << "~Complex()" << endl;
    }

    //输出流运算符不能作为成员函数，因为非静态成员函数的第一个参数是this指针，
    //而输出流运算符的第一个参数是流对象，这样就改变操作数据位置，所以
    //输出流运算符不能以成员函数的形式进行重载
    /* std::ostream &operator<<(std::ostream &os);//error */
    friend std::ostream &operator<<(std::ostream &os, const Complex &rhs);
    friend std::istream &operator>>(std::istream &is, Complex &rhs);

    friend Complex operator+(const Complex &lhs, const Complex &rhs);
private:
    double _dreal;
    double _dimage;
};

//3、运算符重载之友元函数的形式(推荐使用)
Complex operator+(const Complex &lhs, const Complex &rhs)
{
    cout << "friend Complex operator+(const Complex &, const Complex &)" << endl;
    return Complex(lhs._dreal + rhs._dreal
                   , lhs._dimage + rhs._dimage);
}

std::ostream &operator<<(std::ostream &os, const Complex &rhs)
{
    if(0 == rhs._dreal && 0 == rhs._dimage)
    {
        os << 0 << endl;
    }
    else if(0 == rhs._dreal)
    {
        os << rhs._dimage << "i" << endl;
    }
    else
    {
        //1 + i
        os << rhs._dreal;
        if(rhs._dimage > 0)
        {
            os << " + " << rhs._dimage << "i" << endl;
        }
        else if(rhs._dimage < 0)
        {
            os << " - " << (-1)*rhs._dimage << "i" << endl;
        }
        else
        {
            os << endl;
        }
    }

    return os;

}

void readDouble(std::istream &is, double &number)
{
    while(is >> number, !is.eof())
    {
        if(is.bad())
        {
            std::cerr << "The istream is bad" << endl;
            return;
        }
        else if(is.fail())
        {
            is.clear();//重置流的状态
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//
            cout << "请输入一个double类型的数据" << endl;
        }
        else
        {
            cout << "number = " << number << endl;
            break;
        }
    }
}
std::istream &operator>>(std::istream &is, Complex &rhs)
{
    /* is >> rhs._dreal >> rhs._dimage; */
    readDouble(is, rhs._dreal);
    readDouble(is, rhs._dimage);

    return is;
}
/* std::ostream &operator<<(std::ostream &os, const Complex &rhs) */
void test()
{
    Complex c1(1, 2);
    cout << "c1 = " << c1  <<endl;//链式编程,简写
    operator<<(cout, "c1 = ");//原版
    operator<<(operator<<(cout, "c1 = "), c1);
    operator<<(operator<<(cout, "c1 = "), c1).operator<<(endl);

    cout << endl;
    Complex c2;
    std::cin >> c2;
    cout << "c2 = " << c2 << endl;
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

