#include <iostream>

using std::cout;//3、using声明机制,一次只引出一个实体
using std::endl;//推荐使用这种

namespace wd
{
//命名空间中可以定义变量、函数、结构体、命名空间，统称为实体
int number = 10;

void print()
{
    cout << "void print()" << endl;
}

}//end of namespace
#if 0
//2、命名空间+作用域限定符的形式，即使自定义的实体命名空间中的
//实体冲突，一样没有影响
void cout(int x, int y)
{
    std::cout << "x = " << x << ", y = " << y << std::endl;
}
#endif
int main(int argc, char **argv)
{
    cout << "number = " << wd::number << endl;//作用域限定符
    wd::print();
    /* cout(3, 5); */
    return 0;
}

