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

class Cylinder
/* : protected Circle{ */
: public Circle{
public:
    Cylinder();
    Cylinder(double r, double h);
    double getVolume();
    void showVolume();
private:
    double _height;
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

Cylinder::Cylinder()
: Circle(0)
, _height(0){
    
}

Cylinder::Cylinder(double r, double h)
: Circle(r)
, _height(h){

}

double Cylinder::getVolume(){
    return getArea() * _height; 
}

void Cylinder::showVolume(){
    cout << "Volume(" << getArea() << ")" << endl;
}

void test(){
    Circle(10).show();
    Cylinder(100, 10).show();
    Cylinder(100, 10).showVolume(); 
}

int main()
{
    test();
    return 0;
}

