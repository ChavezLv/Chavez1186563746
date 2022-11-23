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

        return *this;//返回本身
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

    cout << endl;
    c1 += c2;
    cout << "c1 = ";
    c1.print();
    
}

void test2()
{
    int a = 3;
    int b = 4;

    cout << "(++a) = " << (++a) << ", a = " << a << endl; //表达式4， a = 4
    cout << "(a++) = " << (a++) << ", a = " << a <<endl;;//表达式4  a = 5
    Complex c1(1, 2);
    cout << "c1 = ";
    c1.print();

    cout << endl;
    Complex c2(3, 4);
    cout << "c2 = ";
    c2.print();

    cout << endl << endl;
    cout << "执行前置++与后置++" << endl;
    cout << "(++c1) = ";
    (++c1).print();//2 + 3i
    cout << "c1 = ";
    c1.print();//2 + 3i
    /* &(++c1);//ok，左值 */

    cout << endl;
    cout << "(c1++) = ";
    (c1++).print();//2 + 3i
    cout << "c1 = ";
    c1.print();//3 + 4i
    /* &(c1++);//error,右值 */
}

void test3()
{
    Complex c1(1, 2);
    cout << "c1 = ";
    c1.print();

    cout << endl;
    Complex c2(1, 0);
    cout << "c2 = ";
    c2.print();

    cout << endl;
    Complex c3(1, -2);
    cout << "c3 = ";
    c3.print();

    cout << endl;
    Complex c4(0, 2);
    cout << "c4 = ";
    c4.print();

    cout << endl;
    Complex c5(0, 0);
    cout << "c5 = ";
    c5.print();

    cout << endl;
    Complex c6(0, -2);
    cout << "c6 = ";
    c6.print();

    cout << endl;
    Complex c7(-1, 2);
    cout << "c7 = ";
    c7.print();

    cout << endl;
    Complex c8(-1, 0);
    cout << "c8 = ";
    c8.print();

    cout << endl;
    Complex c9(-1, -2);
    cout << "c9 = ";
    c9.print();

}
void test4()
{
    Complex c1(1, 2);
    cout << "c1 = " << c1 << c1 <<endl;
    c1.print();
}
int main(int argc, char **argv)
{
    test3();
    return 0;
}

