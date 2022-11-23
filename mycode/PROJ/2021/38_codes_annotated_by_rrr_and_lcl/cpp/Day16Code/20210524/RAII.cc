#include <iostream>

using std::cout;
using std::endl;

template <typename T>//模板类，有一个参数就是：类型
class RAII
{
public:
    //在构造时初始化资源
    RAII(T *data)//传入是时候就传入裸指针
    : _data(data) 
    {
        cout << "RAII(T *)" << endl;
    }
    //提供若干访问资源的方法
    T *operator->()//()符号把对象变为函数，->符号把对象变为资源
    {
        return _data;//返回【指针】去->
    }

    T &operator*()
    {
        return *_data;//解引用返回【自己】//指针 解引用之后的值
    }

    T *get()
    {
        return _data; //get拿到【裸指针】
    }

    void reset(T *data) //reset:释放原来的，且拿到新的 //-->这个就是智能指针的思路啊
    {
        if(_data)//有才delete，没有就不delete了
        {
            delete _data; //释放数据
            _data = nullptr; //置空指针
        }

        _data = data;//重设指针
    }
    //在析构的时候释放资源
    ~RAII()
    {
        cout << "~RAII()" << endl;
        if(_data)
        {
            delete _data; //析构即释放资源 
            _data = nullptr;
        }
    }
    //不允许复制或者赋值
    RAII(const RAII &rhs) = delete;
    RAII &operator=(const RAII &rhs) = delete;
private:
    T *_data; //【指向】T类型资源【的指针】
};

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl; //被用做T的【点类】
    }

    void print()
    {
        cout << "_ix = " << _ix << endl
             << "_iy = " << _iy << endl;
    }
    ~Point()
    {
        cout << "~Point()" << endl;
    }
private:
    int _ix;
    int _iy;
};
int main(int argc, char **argv)
{
    RAII<Point>  pt(new Point(1, 2));//栈对象
    pt->print();

    /* RAII<Point> pt2 = pt;//error */
    RAII<Point> pt3(new Point(3, 4));
    /* pt3 = pt;//error */

    return 0;
}

