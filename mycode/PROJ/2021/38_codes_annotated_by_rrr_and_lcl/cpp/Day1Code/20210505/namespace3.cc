#include <iostream>

/* using namespace std;//1、using编译指令,可以将命名空间中的实体全部引出来 */

namespace wd
{
//命名空间中可以定义变量、函数、结构体、命名空间，统称为实体
int number = 10;

void print()
{
    std::cout << "void print()" << std::endl;
}

}//end of namespace

//2、命名空间+作用域限定符的形式，即使自定义的实体命名空间中的
//实体冲突，一样没有影响
void cout(int x, int y)
{
    std::cout << "x = " << x << ", y = " << y << std::endl;
}

int main(int argc, char **argv)
{
    std::cout << "number = " << wd::number << std::endl;//作用域限定符
    wd::print();
    cout(3, 5);
    return 0;
}

