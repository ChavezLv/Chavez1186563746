#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy) //初始化一个点
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

void test()
{
    //原生指针（裸指针）
    int *pInt = new int(10); //裸指针
    shared_ptr<int> sp(pInt); //裸指针构造shared
    //或者直接用make_shared最好
    cout << "*sp = " << *sp << endl; //解引用
    cout << "sp.get() = " << sp.get() << endl; //拿到指针
    cout << "pInt = " << pInt << endl;//打印指针
    cout << "sp.use_count() = " << sp.use_count() << endl;//引用计数

    cout << endl;
    shared_ptr<int> sp2(sp); //shared_ptr可以拷贝构造函数
    cout << "*sp2 = " << *sp2 << endl;
    cout << "sp2.get() = " << sp2.get() << endl;
    cout << "pInt = " << pInt << endl;
    cout << "sp2.use_count() = " << sp2.use_count() << endl;//查看引用计数(费时)

#if 1
    cout << endl;
    shared_ptr<int> sp3(new int(20));//右值构造sp3
    cout << "*sp3 = " << *sp3 << endl;
    cout << "sp3.get() = " << sp3.get() << endl;
    cout << "pInt = " << pInt << endl;
    cout << "sp3.use_count() = " << sp3.use_count() << endl;

    cout << endl  << "执行赋值操作sp3 = sp" << endl;
    sp3 = sp;//和拷贝一个原理
    cout << "*sp3 = " << *sp3 << endl;
    cout << "sp3.get() = " << sp3.get() << endl;
    cout << "pInt = " << pInt << endl;
    cout << "sp3.use_count() = " << sp3.use_count() << endl;

    cout << endl;
    shared_ptr<Point> sp4(new Point(10, 20)); 
    vector<shared_ptr<Point>> vec;
    vec.push_back(sp4);
    /* vec.push_back(std::move(sp4)); //也是没有问题的*/
    vec.push_back(shared_ptr<Point>(new Point(1, 3)));

    cout << endl;
    Point *pt = new Point(1, 2);
    vector<Point *> number;//不建议使用裸指针
    /* number.push_back(new Point(1, 2)); */
    number.push_back(pt);
    delete pt;

#endif
    //笔记by lcl:\
    1) shared_ptr用了写时复制的机制\
    2) vec.push_back(sp4); //拷贝操作传左值,没有问题\
    vec.push_back(std::move(sp4)); //也是没有问题的,有拷贝构造or移动构造都可以接收
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

