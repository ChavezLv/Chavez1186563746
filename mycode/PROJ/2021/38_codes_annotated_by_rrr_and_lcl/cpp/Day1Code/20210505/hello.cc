#include <stdio.h>
#include <iostream>//C++头文件没有加.h，因为C++头文件都是用模板写的，
                   //而模板有个特点：必须要知道所有实现之后才能正常编译

using namespace std;//命名空间

//函数的声明
//函数声明可以有多次，函数定义只能有一次
void test();
void test();
void test();
void test();
void test();

void test()
{

}

int main(int argc, char **argv)
{
    /* &10;//error,字面值常量,不能取地址 */
    /* &"Hello, world";//ok */
    //cout,标准输出
    //<<,输出流运算符
    //"Hello world",字符串常量
    //endl end of line
    cout << "Hello world" << endl;
    operator<<(cout, "Hello world").operator<<(endl);//运算符重载

    int number = 0;
    //cin，标准输入
    //>>，输入流运算符
    cin >> number;
    cout << "number = " << number << endl;
    return 0;
}

