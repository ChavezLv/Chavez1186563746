#include <math.h>
#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

//抽象类作为接口使用的例子
//可以实现多态
//
//面向对象的设计原则：开闭原则
//对扩展开放，对修改关闭
class Figure
{
public:
    virtual void display() = 0;
    virtual double area() = 0;
};

class Rectangle
: public Figure
{
public:
    Rectangle(double length = 0, double width = 0)
    : _length(length)
    , _width(width)
    {

    }

    void display() override
    {
        cout << "Rectangle";
    }

    double area() override
    {
        return _length * _width;
    }
private:
    double _length;
    double _width;
};

class Circle
: public Figure
{
public:
    Circle(double radius = 0)
    : _radius(radius)
    {

    }

    void display() override
    {
        cout << "Circle";
    }

    double area() override
    {
        return 3.14 * _radius *_radius;;
    }
private:
    double _radius;
};

class Triangle
: public Figure
{
public:
    Triangle(double a = 0, double b = 0, double c = 0)
    : _a(a)
    , _b(b)
    , _c(c)
    {

    }

    void display() override
    {
        cout << "Triangle";
    }

    double area() override
    {
        double tmp = (_a + _b + _c)/2;

        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp - _c));
    }
private:
    double _a;
    double _b;
    double _c;
};

void func(Figure *pfig)
{
    pfig->display();
    cout << "的面积 : " << pfig->area() << endl;
}

class Factory
{
public:
    virtual Figure *create() = 0;
    virtual ~Factory(){}
};

class RectangleFactory
: public Factory
{
public:
    Figure *create() override
    {
        //数据参数都是通过配置文件读取出来的 txt  xml
        return new Rectangle(10, 20);
    }

};

class CircleFactory
: public Factory
{
public:
    Figure *create() override
    {
        //数据参数都是通过配置文件读取出来的 txt  xml
        return new Circle(10);
    }

};

class TriangleFactory
: public Factory
{
public:
    Figure *create() override
    {
        //数据参数都是通过配置文件读取出来的 txt  xml
        return new Triangle(3, 4, 5);
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

