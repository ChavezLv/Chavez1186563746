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


//静态工厂方法
//缺点:1. 不满足开闭原则
//	   2. 不满足单一职责原则
class Factory
{
public:
	static Rectangle * createRectangle()
	{
		//输入数据一般会采用配置文件存储
		//加载配置文件 xml
		//...预处理
		return new Rectangle(10, 11);
	}

	static Circle * createCircle()
	{
		//输入数据一般会采用配置文件存储
		//加载配置文件 xml
		//
		return new Circle (10);
	}

	static Triangle * createTriangle()
	{
		//输入数据一般会采用配置文件存储
		//加载配置文件 xml
		//
		return new Triangle(3, 4, 5);
	}

	static Diomand * createDiomand()
	{
		return new Diomand();
	}
};

int main(void) {
	Rectangle * rectangle = Factory::createRectangle();
	Circle * circle = Factory::createCircle();
	Triangle * triangle = Factory::createTriangle();
	display(rectangle);
	display(circle);
	display(triangle);
	return 0;
}

