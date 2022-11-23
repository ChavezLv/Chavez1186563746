#include <iostream>

using std::cout;
using std::endl;

class Empty
{
//无参构造函数
//拷贝构造函数
//赋值运算符函数
//析构函数
};
int main(int argc, char **argv)
{
    cout << "sizeof(Empty) = " << sizeof(Empty) << endl;

    Empty e1;
    Empty e2;
    Empty e3;
    printf("&e1 = %p\n", &e1);
    printf("&e2 = %p\n", &e2);
    printf("&e3 = %p\n", &e3);
    return 0;
}

