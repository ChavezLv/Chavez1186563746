#include <iostream>

using std::cout;
using std::endl;

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
        cout << "(" << this->_ix 
            << ", "  << this->_iy 
            << ")" <<  endl;
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
    //int number = 10;
    //int *pInt = &number;
    
    Point pt(1, 2);
    Point *ptr = &pt;

    //int *pInt = new int(10);
    Point *pc = new Point(1, 3);
}

void test2()
{
    //int arr[2] = {1, 2};
    Point pt[2] = {Point(1, 2), Point(2, 3)};
    pt[0].print();


    cout << endl;
    //逗号表达式
    /* Point ptemp[2] = {(1, 2), (4, 3)}; */
    Point ptemp[2] = {{1, 2}, {4, 3}};
    ptemp[0].print();
}

void test3()
{
    int *pInt = new int[5]();

    Point *pArr =  new Point[5]();
    cout << "222" << endl;
    pArr[0] = {1, 3};
    cout << "333" << endl;
    pArr[1] = Point(2, 4);//临时对象/匿名对象，生命周期只在本行,对象在本行创建，也在本行销毁
    cout << "111" << endl;

    delete [] pArr;//堆对象数组
    /* free(pArr);//error */
}

int main(int argc, char **argv)
{
    test3();

    return 0;
}

