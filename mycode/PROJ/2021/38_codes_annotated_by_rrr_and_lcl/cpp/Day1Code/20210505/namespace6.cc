#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

int number = 1;

namespace wd
{
int number = 10;

void print(int number)
{
    cout << "形参number = " << number << endl;
    cout << "命名空间中number = " << wd::number << endl;
    cout << "全局number = " << ::number << endl;//匿名命名空间
    printf("helloworld\n");
    ::printf("helloworld\n");
}

//命名空间是可以嵌套的
namespace wh
{
int number = 200;
}

}//end of namespace

int main(int argc, char **argv)
{
    wd::print(20);
    cout << "wh::number = " << wd::wh::number << endl;
    return 0;
}

