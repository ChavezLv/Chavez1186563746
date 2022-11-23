#include <iostream>

using std::cout;
using std::endl;
#if 0
int add(){ return 1; }
int add(int x) { return x + 0 + 1; }
int add(int x,int y) { return x + y + 1; }
int add(int x,int y, int z) { return x + y + z; }
#endif
#if 0
//二义性
int add(int x, int y)
{
    return x + y;
}
#endif
//默认参数要从右往左进行连续的赋初值
int add(int x = 0, int y = 0, int z = 0)
{
    return x + y + z;
}

int main(int argc, char **argv)
{
    int a = 3, b = 4, c = 5;
    cout << "add(a, b, c) = " << add(a, b, c) << endl;
    cout << "add(a, b) = " << add(a, b) << endl;//二义性
    cout << "add(a) = " << add(a) << endl;
    cout << "add() = " << add() << endl;
    return 0;
}

