#include <iostream>

using namespace std;//1、using编译指令,可以将命名空间中的实体全部引出来

#if 0
void HWprint()
{

}

void HFCPPProjectPrint()
{

}
#endif

namespace wd
{
//命名空间中可以定义变量、函数、结构体、命名空间，统称为实体
int number = 10;

void print()
{
    cout << "void print()" << endl;
}

}//end of namespace
/* ; */

//自定义的实体与命名空间中的实体冲突
int cout()
{
    return 10;
}

int main(int argc, char **argv)
{
    cout << "number = " << wd::number << endl;//作用域限定符
    wd::print();
    cout();
    return 0;
}

