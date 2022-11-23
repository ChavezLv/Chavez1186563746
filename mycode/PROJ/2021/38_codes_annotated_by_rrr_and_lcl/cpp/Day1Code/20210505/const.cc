#include <iostream>

using std::cout;
using std::endl;

//宏定义发生的时机是在预处理阶段,字符串替换,有bug会到运行时才会发现
#define MAX 10
#define multiply(x, y) ((x) * (y))


void test()
{
    //发生时机在编译阶段,会进行类型安全检查,如果有bug在编译时候就会出现
    //内置类型：char/short/int/long/double/float/void *
    const int number = 10;//const修饰的变量称为常量，必须在定义的时候进行初始化
    /* number = 20;//赋值,常量不能进行赋值 */

    int const number2 = 20;//[R]和上面等价:两种写法都可以
    //符号常量只能初始化，不能赋值

}

//函数指针           指针函数
//int (*pf)(int)     int*   pf(int)
//
//数组指针           指针数组
//int (*pArray)[]    int* pArray[]

void test2()
{
    int value = 2;
    int value1 = 10;
    int *p1 = &value1;//p1是value1的指针
    p1 = &value;
    *p1 = 20000;

    cout << endl;
    int value2 = 200;
    const int *p2 = &value2;//当const位于*左边的时候，常量指针(pointer to const)
    /* *p2 = 222;//error,不能修改指针所指变量的值 */
    p2 = &value;//ok,可以改变指针本身（指向）

    cout << endl;
    int value3 = 300;
    int const *p3 = &value3;//当const位于*左边的时候，常量指针(pointer to const)
    /* *p3 = 333;//error,不能修改指针所指变量的值 */
    p3 = &value;//ok,可以改变指针本身（指向）

    cout << endl;
    int value4 = 400;
    int * const p4 = &value4;//当const位于*右边的时候，指针常量(const pointer)
    *p4 = 444;//ok,可以修改指针所指变量的值
    /* p4 = &value;//error,不可以改变指针本身（指向） */

    cout << endl;
    int value5 = 500;
    const int * const p5 = &value5;//双const
    /* *p5 = 555;//error,不可以修改指针所指变量的值 */
    /* p5 = &value;//error,不可以改变指针本身（指向） */
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

