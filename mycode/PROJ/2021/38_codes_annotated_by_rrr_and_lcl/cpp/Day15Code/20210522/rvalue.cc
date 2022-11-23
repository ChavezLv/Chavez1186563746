#include <iostream>

using std::cout;
using std::endl;

void test()
{
    int a = 3;
    int b = 4;
    int *pflag = &a;

    int &ref = a; 
    //const左值引用既可以绑定到左值也可以绑定到右值，称为万能引用
    //万能引用, 区分不出是左值还是右值
    // 借助c++11里面的右值引用
    const int &ref2 = 3;
    const int &ref3 = a;

    //右值引用,右值引用只能绑定到右值，不能绑定到左值
    //完美区分左值和右值
    /* int &&rref = b; //error, 不能绑定左值 */
    int &&rref2 = 4;//ok, 只能绑定右值

    &a;
    &b;
    &pflag;
    &*pflag;

    &rref2;//右值引用再此是左值

    /* &(a + b);//error,右值不能取地址 */
}

int arr[10] = {1, 4, 7};
int &getIndex(int idx)
{
    return arr[idx];
}

void test3()
{
    getIndex(0) = 3;
}
void test2()
{
    int number1 = 10;
    int number2 = 20;
    number2 = std::move(number1);
    cout << "number1 = " << number1 << endl;
    cout << "number2 = " << number2 << endl;
}

//右值引用既可以是左值也可以是右值
int &&func()
{
    return 10;
}
int main(int argc, char **argv)
{
    test2();
    &func();
    return 0;
}

