#include <iostream>

/* using namespace std;//1、using编译指令,一次可以将std中的实体 */
                    //全部引出来
namespace wd
{
//命名空间中可以定义变量、函数、结构体、类,统称为实体
int number = 10;

void print()
{
    std::cout << "void print()" << std::endl;
}


}//end of namespace wd

//自定义的实体与std命名空间中的实体冲突了
void cout(int a)
{
    std::cout << "a =" << a << std::endl;
}
int main(int argc, char **argv)
{
    std::cout << "number = " << wd::number << std::endl;//作用域限定符
    wd::print();
    cout(10);
    return 0;
}


