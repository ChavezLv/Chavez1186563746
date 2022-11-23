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

class Figure {
public:
    Figure() : _name(string("")) {
        /* cout << "Figure()" << endl; */ 
    }
    Figure(string name) : _name(name){
        /* cout << "Figure(string name) : _name(name)" << endl; */
    }
    virtual
    ~Figure(){

    }
    virtual double getArea() const = 0;
    virtual string getName() const = 0;
    virtual void show() const = 0;
protected:
    string _name;

};

class Circle 
: public Figure{
protected:
#define PI 3.14159265
    double _r;

public:
    Circle() ;//:  _r(0){  }
    Circle(double r, string name = "");// : Figure(name), _r(r){  }

    //获取圆的面积
    double getArea() const;
    //获取圆的周长    
    double getPerimeter() const;
    //将圆的半径、周长、面积输出到屏幕
    void show() const;

    string getName() const;
};

class Cylinder
/* : protected Circle{ */
: public Circle{
public:
    Cylinder();
    Cylinder(double r, double h);
    double getVolume() const;
    void showVolume();
    double getArea() const;
    void show() const;
private:
    double _height;

};


Circle::Circle()
    : _r(0){


    }

Circle::Circle(double r, string name)
    : Figure(name),
     _r(r){


    }

double Circle::getArea() const{
    return PI * _r * _r;

}

double Circle::getPerimeter() const{
    return 2 * PI * _r;

}

string Circle::getName() const{
    return _name;
}

void Circle::show() const{
    cout << this << ": " << "r(" << _r << "), perimeter(" << \
        getPerimeter() << "), area(" << getArea() << ")" << endl;

}

Cylinder::Cylinder()
    : Circle(0)
      , _height(0){
        cout << "Cylinder()" << endl;
      }

Cylinder::Cylinder(double r, double h)
    : Circle(r)
      , _height(h){
            cout << "Cylinder(double r, double h)" << endl;

      }

double Cylinder::getArea() const{
    return Circle::getArea() * 2 + Circle::getPerimeter() * _height;
}

void Cylinder::show() const{
    cout << this << ": " << "r(" << _r << "), perimeter(" << \
        getPerimeter() << "), area(" << getArea() << "), volume(" << \
        getVolume() << ")" << endl;

}

double Cylinder::getVolume() const{
    return Circle::getArea() * _height; 
}

void Cylinder::showVolume(){
    cout << "Volume(" << getVolume() << ")" << endl;

}

void test(){
    Circle(10).show();
    Cylinder(100, 10).show();
    Cylinder(100, 10).showVolume(); 

}

void test2(){
    Cylinder cyl(3, 10); 
    Cylinder *pCylinder = &cyl;//new Cylinder(3, 10);
    cout << pCylinder->getArea() << endl;
    cout << pCylinder->getName() << endl;
    pCylinder->show();
    pCylinder->showVolume();

    Circle *pCircle = &cyl;
    cout << pCircle->getArea() << endl;

    Figure *pFigure = &cyl;
    cout << pFigure->getArea() << endl;
}
int main()
{
    /* test(); */
    test2();
    return 0;

}
