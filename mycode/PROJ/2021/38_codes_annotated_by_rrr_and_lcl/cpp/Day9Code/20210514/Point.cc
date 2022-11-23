#include <math.h>
#include <iostream>
#include <limits>

using std::cout;
using std::endl;

class Point;

class Complex
{
    friend class Point;
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

    friend std::ostream &operator<<(std::ostream &os, const Complex &rhs);
    friend std::istream &operator>>(std::istream &is, Complex &rhs);
    friend Complex operator+(const Complex &lhs, const Complex &rhs);
private:
    double _dreal;
    double _dimage;
};

Complex operator+(const Complex &lhs, const Complex &rhs)
{
    cout << "friend Complex operator+(const Complex &, const Complex &)" << endl;
    return Complex(lhs._dreal + rhs._dreal, lhs._dimage + rhs._dimage);
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
class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }

    Point(const Complex &rhs)
    : _ix(rhs._dreal)
    , _iy(rhs._dimage)
    {

    }

    void print() const
    {
        cout << "("   << _ix 
             << ", "  << _iy 
             << ")"   <<  endl;
    }

    //1、必须是一个成员函数
    //2、没有返回类型
    //3、目标类型写在operator后面
    //4、通过return去返回目标类型的变量
    operator int()
    {
        cout << "operator int()" << endl;
        return _ix + _iy;
    }

    operator double()
    {
        if(0 == _iy)
        {
            return 0;
        }
        else
        {
            return static_cast<double>(_ix)/_iy;
        }

    }

    operator Complex()
    {
        cout << "operator Complex()" << endl;
        return Complex(_ix, _iy);
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }

private:
    int _ix;
    int _iy;
};

void test()
{
    Point pt1(1, 2);
    cout << "pt1 = ";
    pt1.print();

    //从其他类型向自定义类型进行转换（其他类型可以是内置类型，也可以是其他的
    //自定义类型）
    //解决办法：利用自定义类型的构造函数作为中转
    cout << endl;
    Point pt2 = 5;//Point(5, 0)
    cout << "pt2 = ";
    pt2.print();

    cout << endl;
    Complex c1(1, 4);
    Point pt3 = c1;//Point(c1._dreal, c1._dimag)
    cout << "pt3 = ";
    pt3.print();
}

void test2()
{
    Point pt1(1, 2);
    cout << "pt1 = ";
    pt1.print();

    //从自定义类型向其他类型进行转化
    cout << endl;
    int ix = pt1;
    cout << "ix = " << ix << endl;

    double dy = pt1;
    cout << "dy = " << dy << endl;

    Complex c2 = pt1;
    cout << "c2 = " << c2 << endl;
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}
