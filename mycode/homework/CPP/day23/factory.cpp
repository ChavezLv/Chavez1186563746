#include<math.h>
#include<string>
#include<iostream>
#include<memory>
using namespace std;
class Figure{
public:
    virtual void display()=0;
    virtual double area()=0;
};
class Rectangle:public Figure{
public:
    Rectangle(double length=0,double width =0)
    :_length(length),_width(width){}
    void display()override
    {
        cout << "Rectangle";
    }
    double area ()override
    {
        return _length*_width;
    }
private:
    double _length;
    double _width;
};
class Circle
:public Figure {
public:
    Circle(double radius=0):_radius(radius){}
    void display()override{
        cout<<"Circle";
    }
    double area()override{
        return 3.14*_radius*_radius; }
private:
    double _radius;
};
class Triangle
: public Figure
{
public:
    Triangle(double a = 0, double b = 0, double c = 0)
    : _a(a) , _b(b), _c(c){}

    void display() override{
        cout << "Triangle";
    }
    double area() override{
        double tmp = (_a + _b + _c)/2;
        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp - _c));
    }
private:
    double _a;
    double _b;
    double _c;
};
void func(Figure *pfig){
    pfig->display();
    cout<<"的面积:"<<pfig->area()<<endl;
}
/* 违背了单一职责原则
 * 违背了开放闭合原则*/

class Factory{
public:
    static Figure * create(const string &type){
        if(type=="rectangle"){
            return new Rectangle(10,20);
        }
        else if(type =="circle"){
            return new Circle(20);
        }
        else if (type =="triangle"){
            return new Triangle(3,4,5);
        }
        else {return nullptr;}
    }
};
int main(int argc, char **argv)
{
    /* Rectangle rectangle(10, 20); */
    /* Circle circle(10); */
    /* Triangle triangle(3, 4, 5); */
    unique_ptr<Figure> rec(Factory::create("rectangle"));
    unique_ptr<Figure> cir(Factory::create("circle"));
    unique_ptr<Figure> tri(Factory::create("triangle"));

    func(rec.get());
    func(cir.get());
    func(tri.get());
    return 0;
}
