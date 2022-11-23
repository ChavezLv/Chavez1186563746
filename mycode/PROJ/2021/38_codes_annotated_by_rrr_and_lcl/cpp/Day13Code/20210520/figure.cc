#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

//在面向对象的设计中，如何通过很小的设计改变就可以应对设计需求的变化，这是设计者们极为关注的问题。为此不少OO先驱提出了很多有关面向对象的设计原则用于指导OO的设计和开发。
//面向对象的设计原则：开闭原则
//特点：就是对扩展开放，对修改关闭

//抽象类
class Figure
{
public:
    virtual 
    void display() const = 0;//纯虚函数，作为接口存在，具体实现交给派生类去完成
    //开闭原则就是 这里的接口不允许作任何修改, 否则后面的就会有一大串报错
    virtual 
    double area() const = 0; 
};


class Rectangle
: public Figure
{
public:
    Rectangle(double length, double width)
    : _length(length)
    , _width(width)
    {

    }

    void display() const override
    {
        cout << "Rectangle";
    }

    double area() const override
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
    Circle(double radis)
    : _radis(radis)
    {

    }

    void display() const override
    {
        cout << "Circle";
    }

    double area() const override
    {
        return _radis * _radis * 3.1415;
    }
private:
    double _radis;
};

class Traingle
: public Figure
{
public:
    Traingle(double a, double b, double c)
    : _a(a)
    , _b(b)
    , _c(c)
    {

    }

    void display() const override
    {
        cout << "Traingle";
    }

    //海伦公式
    double area() const override
    {
        double tmp = (_a + _b + _c)/2;

        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp - _c));
    }
private:
    double _a;
    double _b;
    double _c;
};

void func(const Figure &fig)
{
    fig.display();
    cout << "'s area is : " << fig.area() << endl;
}

int main(int argc, char **argv)
{
    Rectangle rectangle(10, 20);
    Circle circle(10);
    Traingle traingle(3, 4, 5);

    func(rectangle);
    func(circle);
    func(traingle);

    return 0;
}
