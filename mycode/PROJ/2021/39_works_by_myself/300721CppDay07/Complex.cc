#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
void test();
void test1();
class Override;
class Complex{
public:
    Complex(double dreal = 0, double dimage = 0)
    : _dreal(dreal)
    , _dimage(dimage){
        
    }

    void print(){
        cout << _dreal << "+" << _dimage << endl;
    }

    // +=    a+=b;  a = a + b;
    Complex &operator+=(const Complex &rhs){
        _dreal += rhs._dreal;
        _dimage += rhs._dimage;
        return *this;
    }
    //++   ++a   a = a + 1;
    Complex &operator++(){
        ++_dreal;
        ++_dimage;
        return *this;
    }
    //a-- (a) --a--
    Complex operator++(int){
        Complex tmp(*this);
        ++_dreal;
        ++_dimage;

        

        return tmp;
    }

    friend Complex operator+(const Complex &lhs, const Complex &rhs);
    friend Complex operator-(const Complex &lhs, const Complex &rhs);

private:
    double _dreal;
    double _dimage;
};

class Override{
    /* Complex operator+(const Complex &lhs, const Complex &rhs){ */
    /*     return Complex(rhs._dreal + lhs._dreal, lhs._dreal + rhs._dimage); */
    /* } */

};

Complex operator+(const Complex &lhs, const Complex &rhs){
    // 返回类型 (c1 + c2) --> Complex
    return Complex(rhs._dreal + lhs._dreal, lhs._dreal + rhs._dimage);
}

Complex operator-(const Complex &lhs, const Complex &rhs){
    return Complex(rhs._dreal - lhs._dreal, lhs._dreal - rhs._dimage);
}

int main()
{
    test1();
    return 0;
}

void test1(){
    /* Complex cpl = (1, 2);//(2,0) */ 
    /* Complex cpl = {1, 2}; //(1,2) */
    Complex c1(1, 2), c2(3, 4);
    /* c1 = c1 + c2;//correct */
    c1 += c2;//correct
    c1.print();
    ++c1;

    c1.print();
}
void test(){
    stringstream ss;
    ss << __LINE__;
    string str1;
    ss >> str1;
    cout << str1 << endl;
}
