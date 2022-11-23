#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Circle {
#define PI 3.14159265
public:
    Circle();
    Circle(double r); 

    //获取圆的面积
    double getArea();
    //获取圆的周长    
    double getPerimeter();
    //将圆的半径、周长、面积输出到屏幕
    void show();

private:
    double _r;
};

Circle::Circle()
: _r(0){

}

Circle::Circle(double r)
: _r(r){
    
}

double Circle::getArea(){
    return PI * _r * _r;
}

double Circle::getPerimeter(){
    return 2 * PI * _r;
}

void Circle::show(){
    cout << this << ": " << "r(" << _r << "), perimeter(" << \
        getPerimeter() << "), area(" << getArea() << ")" << endl;
}

void test(){
    Circle(10).show();
}
int main()
{
    test();
    return 0;
}

