#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::weak_ptr;
using std::shared_ptr;

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
// 笔记by lcl:\
weak_ptr<Point> wp(new Point(1, 3));// error\
看cppref网站可知, weak_ptr要么是weak_ptr要么是shared_ptr作为传值,\
 或者智能创建一个无参对象\
特点: 不能跟直接从普通裸指针转换,不能直接跟所托管的资源直接联系\
1)没有箭头运算符, 或者解引用运算符的重载, 那怎么办? \
wp-> print(); //error\
使用成员函数lock()\
std::weak_ptr<T>::lock\
std::shared_ptr<T> lock() const noexcept;//返回的是shared_ptr\
所以\
shared_ptr<Point> sp2 = wp.lock();\

//接下来学习删除器
void test()
{
    weak_ptr<Point> wp; //无参对象
    {
        shared_ptr<Point> sp(new Point(1, 3));
        wp = sp; //用的时weak_ptr的赋值运算符函数
        cout << "sp.use_count() = " << sp.use_count() << endl;//1
        cout << "wp.use_count() = " << wp.use_count() << endl;//1
        //cout << "*sp=";
        //sp->print();//1


        cout << endl << endl;
        cout << "wp.expired() = " << wp.expired() << endl;//0, 表示资源还在
        shared_ptr<Point> sp2 = wp.lock();
        /* if(_brand) */
        if(sp2)
        {
            cout << "提升成功" << endl;
            cout << "sp2 = ";
            sp2->print();
        }
        else
        {
            cout << "提升失败" << endl;
        }
    }//笔记by lcl: sp生命周期销毁

    cout << endl << endl;
    cout << "wp.expired() = " << wp.expired() << endl;//1, 表示资源没了
    shared_ptr<Point> sp2 = wp.lock();//提升为so
    /* if(_brand) */
    if(sp2)
    {
        cout << "提升成功" << endl;
        cout << "sp2 = ";
        sp2->print();
    }
    else
    {
        cout << "提升失败" << endl;
    }
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

