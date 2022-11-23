#include <stdlib.h>
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

void test2()
{
    int number = 10;
    printf("sizeof(number) = %lu", sizeof(number));
    printf("sizeof number = %lu", sizeof number);//sizeof是一个运算符
}
//面试中常问的
//内存溢出？踩内存？内存越界？野指针
//
//
//面试题
//malloc/free与new/delete异同点？
//1、都是用来申请堆空间的
//2、malloc与free，new与delete要成对出现，否则可能造成内存泄漏
//
//不同点：
//1、malloc/free是C里面的库函数，new/delete是C++中的表达式
//2、malloc申请的是未初始化的堆空间，new申请是已经初始化的堆空间

void test()
{
    int *pInt = (int *)malloc(sizeof(int));//1、申请堆空间
    memset(pInt, 0, sizeof(int));//2、初始化
    *pInt = 10;//3、赋值

    free(pInt);//4、释放堆空间
    /* pInt = NULL;//0 */
    pInt = nullptr;//void *

    int *pArray = (int *)(malloc(sizeof(int) * 10));
    memset(pArray, 0, sizeof(int) * 10);

    free(pArray);
    pArray = nullptr;
}

void test3()
{
    int *pInt = new int(10);//1、申请堆空间，并初始化，还可以进行赋值
    cout << "*pInt = " << *pInt << endl;

    delete pInt;//2、释放堆空间
    pInt = nullptr;

    int *pArray = new int[10]();
    pArray[0] = 120;

    delete [] pArray;
}
int main(int argc, char **argv)
{
    cout << "Hello world" << endl;
    return 0;
}

