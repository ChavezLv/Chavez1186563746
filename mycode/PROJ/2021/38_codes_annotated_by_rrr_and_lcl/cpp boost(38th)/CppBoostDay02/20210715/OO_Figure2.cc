 ///
 /// @file    Figure.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-28 14:31:43
 ///
 
#include <math.h>
#include <iostream>
#include <string>
#include <memory>
using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

class Figure
{
public:
	virtual void display() const =0;
	virtual double area()=0;

	virtual ~Figure() {}
};

class Circle
: public Figure
{
public:
	Circle(double radius)
	: _radius(radius)
	{}

	void display() const
	{	cout << "circle";	}

	double area()
	{
		return 3.14159 * _radius * _radius;
	}

	~Circle() {	cout << "~Circle()" << endl;	}

private:
	double _radius;
};

class Rectangle
: public Figure
{
public:
	Rectangle(double length, double width)
	: _length(length)
	, _width(width)
	{}

	~Rectangle() { cout << "~Rectangle()" << endl; }

	void display() const
	{	cout << "rectangle";	}

	double area()
	{	return _length * _width;	}

private:
	double _length;
	double _width;
};

class Triangle
: public Figure
{
public:
	Triangle(double a, double b, double c)
	: _a(a)
	, _b(b)
	, _c(c)
	{}

	~Triangle() {	cout << "~Triangle()" << endl;	}

	void display() const
	{	cout << "triangle";}

	double area()
	{
		double p = (_a + _b + _c) / 2;
		return sqrt(p * (p - _a) * (p - _b) * (p - _c));
	}

private:
	double _a;
	double _b;
	double _c;
};

void display(Figure * fig)
{
	fig->display();
	cout << "的面积是:" << fig->area() << endl;
}

class Diomand : public Figure
{
public:
	void display() const
	{	cout << "diomand ";	}

	double area() 
	{	return 10.111;}
};


//工厂模式
//使用场景:
//1. 简单对象不需要使用工厂模式
//2. 复杂对象才需要使用工厂模式
//3. 对于不确定的对象，可以使用工厂模式
class Factory
{
public:
	virtual Figure * create() = 0;
	/* virtual Hero * createHero() = 0; */

	virtual ~Factory() {}
};

class RectangleFactory
: public Factory
{
public:
	Figure * create()
	{
		//输入数据一般会采用配置文件存储
		//加载配置文件 xml
		//...预处理
		return new Rectangle(10, 11);
	}

	~RectangleFactory() {	cout << "~RectangleFactory()" << endl;	}
};

class CircleFactory 
: public Factory
{
public:
	Figure * create()
	{
		//输入数据一般会采用配置文件存储
		//加载配置文件 xml
		//
		return new Circle(10);
	}

	~CircleFactory() {	cout << "~CircleFactory()" << endl;	}
};

class TriangleFactory
: public Factory 
{
public:
	Figure * create()
	{
		//输入数据一般会采用配置文件存储
		//加载配置文件 xml
		//
		return new Triangle(3, 4, 5);
	}

	~TriangleFactory() {	cout << "~TriangleFactory()" << endl;	}
};

Figure * createFig(Factory * fac)
{
	return fac->create();
}

int main(void) {
	unique_ptr<Factory> rectangleFactory(new RectangleFactory());
	unique_ptr<Figure> rectangle(rectangleFactory->create());

	unique_ptr<Factory> circleFactory(new CircleFactory());
	unique_ptr<Figure> circle(circleFactory->create());

	unique_ptr<Factory> triangleFactory(new TriangleFactory());
	unique_ptr<Figure> triangle(triangleFactory->create());
	display(rectangle.get());
	display(circle.get());
	display(triangle.get());

	/* Computer com("Huawei Matebook", 8000); */

	/* Figure * fig = createFig(factory); */
	return 0;
}

