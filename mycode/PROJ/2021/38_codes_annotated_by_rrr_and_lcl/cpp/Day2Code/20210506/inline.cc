#include <iostream>

using std::cout;
using std::endl;

#define multiply(x, y) ((x) * (y))
/* #define multiply(x, y) x * y */

//普通函数会有参数入栈出栈的开销

//现代的编译器可以自动识别一个函数是不是内联函数
//特点：在函数调用的时候，用函数体去代替函数的调用
inline int add(int x, int y)//内联函数：如果有多个文件，请保证多个内联的实现一样
{
    return x + y;
}

int main(int argc, char **argv)
{
    int a = 3, b = 4, c = 5, d = 6;
    cout << "multiply(a, b) = " << multiply(a, b) << endl;
    //a + c * b + d = 3 + 5 * 4 + 6
    cout << "multiply(a + c, b + d) = " << multiply(a + c, b + d) << endl;
    cout << "add(a, b) = " << add(a, b) << endl;
    return 0;
}

