#include <stdlib.h>
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//C++支持函数重载，C语言不支持函数重载
//函数重载原理：对函数名字做了改编(name mangling)
//函数重载步骤：在同一个作用域中，当函数名字相同的时候，根据参数的
//个数、顺序、类型对函数名字进行改编
int add(int x, int y)
{
    return x + y;
}

int add(int x, int y, int z)
{
    return x + y + z;
}

float add(float x, float y)
{
    return x + y;
}

float add(float x, float y, float z)
{
    return x + y + z;
}

float add(float x, int y, float z)
{
    return x + y + z;
}

int main(int argc, char **argv)
{
    int *pInt = static_cast<int *>(malloc(sizeof(int)));
    memset(pInt, 0, sizeof(int));

    free(pInt);
    pInt = nullptr;
    return 0;
}

