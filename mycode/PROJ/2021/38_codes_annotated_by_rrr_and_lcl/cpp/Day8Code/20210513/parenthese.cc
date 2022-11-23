#include <iostream>

using std::cout;
using std::endl;


//重载了函数调用运算符的类创建的对象，称为函数对象
//
class FunctionObject
{
public:
    FunctionObject()
    : _cnt(0)
    {

    }

    int operator()(int x, int y)
    {
        cout << "int operator()(int, int)" << endl;
        ++_cnt;
        return x + y;
    }

    int operator()(int x, int y, int z)
    {
        cout << "int operator()(int, int, int)" << endl;
        ++_cnt;
        return x * y * z;
    }
private:
    int _cnt;//函数对象的状态
};

int func(int x, int y)
{
    static int cnt = 0;
    ++cnt;
    return x + y;
}
int main(int argc, char **argv)
{
    int a = 3, b = 4, c = 5;
    FunctionObject fo;
    //fo.operator()(a, b);
    cout << "fo(a, b) = " << fo(a, b) << endl;
    //fo.operator()(a, b, c);
    cout << "fo(a, b, c) = " << fo(a, b, c) << endl;

    cout << "func(a, b) = " << func(a, b) << endl;

    return 0;
}

