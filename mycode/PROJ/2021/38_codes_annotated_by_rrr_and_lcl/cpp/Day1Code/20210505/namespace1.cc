#include <iostream>

using namespace std;

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
//命名空间中可以定义变量、函数、结构体、命名空间，统称为实体//都是实体啊
int number = 10;

void print()
{
    cout << "void print()" << endl;
}

}//end of namespace
/* ; */

int main(int argc, char **argv)
{
    cout << "number = " << wd::number << endl;//作用域限定符
    wd::print();
    return 0;
}

