#include <math.h>
#include <iostream>
#include <functional>

using std::cout;
using std::endl;
using std::function;

//course1 
// 笔记by lcl:
//#include <functional>
//-----------------把函数类型装起来-----------------------
/*
一)方式: 
        using DisplayCallback = function<void()>;//c++11的写法, 用DisplayCallback取代

        等同于: typedef function<void()> DisplayCallback; 



    接下来再用类型创建对象:

    DisplayCallback _displayCallback;

    再将抽象类写成回调函数的形式(框架搭好):

    然后派生类就不用加上virtual和override了

    
二) 对比: 不需要纯虚 + 不需要继承 + 函数名和函数参数不受限制
    因为bind可以改变函数的形态

*/



//function + bind实现多态
class Figure//本来是base类(提供被继承的底层接口)，现在是figure类(提供工具锤子平台)
{
public:
    /* typedef function<void()> DisplayCallback;//C语言typedef */

    using DisplayCallback = function<void()>; //注册【函数类型】 //std::function，又是一个升级版函数指针一样的东西
    using AreaCallback = function<double ()>;

    DisplayCallback _displayCallback; //定义【call wrapper】 //可以借它实际调用到函数
    AreaCallback _areaCallback;

    //注册回调函数
    void setDisplayCallback(DisplayCallback &&cb)//注册回调函数
    {
        _displayCallback = std::move(cb);//即【把传进来的值变为右值给_displayCallback】//函数指针赋值
    }

    void setAreaCallback(AreaCallback &&cb)
    {
        _areaCallback = std::move(cb);
    }

    //执行回调函数
    void handDisplayCallback() const //回调函数
    {
        if(_displayCallback)//如果指针有内容
        {
            _displayCallback();//直接当成函数调用就行
        }
    }

    double handAreaCallback() const //回调函数
    {
        if(_areaCallback)
        {
            _areaCallback();
        }
        else
        {
            return 0;//否则，返回是0
        }
    }
#if 0
    void display() const = 0;//纯虚函数，作为接口存在，具体实现交给派生类去完成

    virtual 
    double area() const = 0; //本来的做法【基类里放继承接口】
    //现在的做法【基类里放<工具锤子的接口>】--->回调函数的位置
#endif
};


class Rectangle
{
public:
    Rectangle(double length, double width) //Rectangle位
    : _length(length) //长度
    , _width(width)
    {

    }

    void display(int x) const
    {
        cout << "Rectangle";
    }

    double area() const //成员函数照常写，只是没有了【继承】，但其他都没有区别
    {
        return _length * _width;
    }
private:
    double _length;
    double _width;
};

class Circle
{
public:
    Circle(double radis)
    : _radis(radis)
    {

    }

    void display() const 
    {
        cout << "Circle";
    }

    double area() const 
    {
        return _radis * _radis * 3.1415;
    }
private:
    double _radis;
};

class Traingle
{
public:
    Traingle(double a, double b, double c)
    : _a(a)
    , _b(b)
    , _c(c)
    {

    }

    void displayTri() const 
    {
        cout << "Traingle";
    }

    //海伦公式
    double areaTri() const 
    {
        double tmp = (_a + _b + _c)/2;

        return sqrt(tmp * (tmp - _a) * (tmp - _b) * (tmp - _c));
    }
private:
    double _a;
    double _b;
    double _c;
};

void func(const Figure &fig) //func函数完成了【如何通过fig】【调用2个函数(display, area)】
{
    fig.handDisplayCallback();//纯打印名字
    cout << "'s area is : " << fig.handAreaCallback() << endl;

}


int main(int argc, char **argv)
{
    Rectangle rectangle(10, 20);
    Circle circle(10);
    Traingle traingle(3, 4, 5);

    Figure fig;
    // 传入:  void()
    fig.setDisplayCallback(std::bind(&Rectangle::display, &rectangle, 10));//先自己设定fig的函数工具锤子
    fig.setAreaCallback(std::bind(&Rectangle::area, &rectangle));
    func(fig);//然后直接用

    fig.setDisplayCallback(std::bind(&Circle::display, &circle));
    fig.setAreaCallback(std::bind(&Circle::area, &circle));
    func(fig);

    fig.setDisplayCallback(std::bind(&Traingle::displayTri, &traingle));
    fig.setAreaCallback(std::bind(&Traingle::areaTri, &traingle));
    func(fig);



    return 0;
}
