#include <iostream>

using std::cout;
using std::endl;

class A
{
public:
    A()
    {
        cout << "A()" << endl;
    }

    A(int ia)
    : _ia(ia)
    {
        cout << "A(int )" << endl;
    }

    ~A()
    {
        cout << "~A()" << endl;
    }
protected:
    int _ia;
};

class B
: virtual public A
{
public:
    B()
    {
        cout << "B()" << endl;
    }

    B(int ia, int ib)
    : A(ia)
    , _ib(ib)
    {
        cout << "B(int, int)" << endl;
    }

    ~B()
    {
        cout << "~B()" << endl;
    }
protected:
    int _ib;
};

class C
: public B
{
public:
    C(int ia, int ib, int ic)
    : B(ia, ib)
    , A(ia)//虚基类A的初始化, 最终要交个派生类, 
    //因为虚拟继承只会在整个布局里面存一份, 这样c和a是有关系的
    //让C去初始化, 避免混淆
    //课件里面有具体原因, 如果没有显示调用, 调用的是默认的, 没有写出来就会出错
    , _ic(ic)
    {
        cout << "C(int, int, int)" << endl;
    }
    //还要注意顺序
    void print() const
    {
        cout << "_ia = " << _ia << endl
             << "_ib = " << _ib << endl
             << "_ic = " << _ic << endl;
    }

    ~C()
    {
        cout << "~C()" << endl;
    }
private:
    int _ic;
};
    //多个基类, 多个虚函数指针, 多个虚表, 效率会稍稍降低, 但是灵活性高些
int main(int argc, char **argv)
{
    C c(10, 20, 30);
    c.print();
    return 0;
}

