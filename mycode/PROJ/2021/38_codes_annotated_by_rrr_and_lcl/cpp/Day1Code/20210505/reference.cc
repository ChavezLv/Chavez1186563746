#include <iostream>

using std::cout;
using std::endl;

//指针与引用的异同点？
void test()
{
    int number = 10;
    int &ref = number;//引用是变量的别名,引用的提出就是为了减少指针的使用
    &ref;
    cout << "number = " << number << endl;
    cout << "ref = " << ref << endl;
    printf("number = %p\n", &number);
    printf("ref = %p\n", &ref);

    cout << endl;
    int number2 = 200;
    ref = number2;//操作引用与操作变量本身是一样的
    cout << "number2 = " << number2 << endl;
    cout << "number = " << number << endl;
    cout << "ref = " << ref << endl;
    printf("number2 = %p\n", &number2);
    printf("number = %p\n", &number);
    printf("ref = %p\n", &ref);

    cout << endl;
    //引用的实质：指针常量 * const
    /* int &ref2;//引用不能独立存在，在定义的时候必须要进行初始化,在定义的 */
              //的时候绑定到变量上面，跟变量绑定到一起，不会改变引用的指向
}

//1、引用作为函数参数
#if 0
//值传递====副本
//没有触及a b本身
void swap(int x, int y)//int x = a, int y = b
{
    int temp = x;
    x = y;
    y = temp;
}
#endif
#if 0
//值传递====地址值
void swap(int *px, int *py)//int *px = &a, int *py = &b;
{
    int temp = *px;
    *px = *py;
    *py = temp;
}
#endif
//引用传递====变量本身
void swap(int &x, int &y)//int &x = a, int &y = b
{
    int temp = x;
    x = y;
    y = temp;
}

void test2()
{
    int a = 3, b = 4;
    cout << "在交换之前 a = " << a << ", b = " << b << endl; 
    swap(a, b);
    cout << "在交换之后 a = " << a << ", b = " << b << endl; 
}

//2、引用作为函数返回类型

int func1()
{
    int number = 10;
    return number;//执行一个拷贝操作
}

int &func2()
{
    int number = 10;//局部变量
    return number;//不能返回一个局部变量的引用
}

//不要去返回堆空间的引用,必须要有内存回收的机制
int &getHeapData()
{
    int *pInt = new int(100);

    return *pInt;
}

void test4()
{
    int a = 3, b = 5;
    int temp = a + getHeapData() + b;
    cout << "temp = " << temp << endl;

    int &ref = getHeapData();
    delete &ref;
}

//函数返回类型是引用的前提:实体的生命周期一定要大于函数的生命周期
int arr[10] = {1, 3, 5, 7, 9, 10};
int &getIndex(int idx)
{
    return arr[idx];//先不去考虑越界
}

void test3()
{
    cout << "getIndex(0) = " << getIndex(0) << endl;
    getIndex(0) = 200;
    cout << "getIndex(0) = " << getIndex(0) << endl;
    cout << "arr[0] = " << arr[0] << endl;
    
    /* func1() = 200; */
}
int main(int argc, char **argv)
{
    test4();
    return 0;
}

