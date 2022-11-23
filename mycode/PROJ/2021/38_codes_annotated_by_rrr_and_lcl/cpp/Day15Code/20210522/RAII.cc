#include <iostream>

using std::cout;
using std::endl;

template <typename T>//模板
class RAII
{
public:
    //一)在构造时(对象构建时)初始化资源
    RAII(T *data)
    : _data(data)
    {
        cout << "RAII(T *)" << endl;
    }
    //二)提供若干访问资源的方法
    //箭头访问
    T *operator->()
    {
        return _data;
    }
    //解引用
    T &operator*()
    {
        return *_data;
    }
    //访问底层数据
    T *get()
    {
        return _data;
    }

    void reset(T *data)
    {
        if(_data)
        {
            delete _data;
            _data = nullptr;
        }

        _data = data;
    }
    //三)在析构的时候释放资源
    ~RAII()
    {
        cout << "~RAII()" << endl;
        if(_data)
        {
            delete _data;
            _data = nullptr;
        }
    }
    //四)不允许复制或者赋值
    RAII(const RAII &rhs) = delete;
    RAII &operator=(const RAII &rhs) = delete;
private:
    T *_data;
};

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
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
    RAII<int> raii(new int(10));//
    RAII<Point>  pt(new Point(1, 2));//栈对象
    pt->print();//pt时栈对象, 类似于智能指针

    RAII<Point> pt2 = pt;//error
    RAII<Point> pt3(new Point(3, 4));
    pt3 = pt;//error

    return 0;
}

