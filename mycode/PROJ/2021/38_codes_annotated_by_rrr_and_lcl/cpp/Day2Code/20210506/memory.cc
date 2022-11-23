#include <iostream>

using std::cout;
using std::endl;

int a;//全局变量，位于全局区,初始化为0 //全局变量：位于全局区初始化为0
char *p1;//全局变量，位于全局区,初始化为nullptr 
const int kMax = 100;//const变量只接受初始化，不接受之后再更改值

int main(int argc, char **argv)
{
    const int kMin = 0;//局部常量位于栈上 //局部常量量位于栈上
    int b;//局部变量，位于栈区,初始化为随机值
    char *p2;//p2本身也是位于栈区
    char str1[] = "hello";//str1位于栈上 //但是我估计定义为*pstr就会被【放在文字常量区】
    static int sc = 10;//静态变量位于静态区

    int *pInt = new int(10);//pInt本身位于栈上， pInt指向堆区
    const char *pstr = "helloworld";//pstr本身位于栈上，pstr指向变量位于文字常量区的

    printf("\n打印变量的地址\n");
    printf("&a = %p\n", &a);
    printf("&p1 = %p\n", &p1);
    printf("p1 = %p\n", p1);
    printf("&b = %p\n", &b);
    printf("&p2 = %p\n", &p2);
    printf("p2 = %p\n", p2);
    printf("&str1 = %p\n", &str1);
    printf("str1 = %p\n", str1);
    printf("&sc = %p\n", &sc);
    printf("&pInt = %p\n", &pInt);
    printf("pInt = %p\n", pInt);
    printf("&pstr = %p\n", &pstr);
    printf("pstr= %p\n", pstr);
    printf("\"helloworld\"= %p\n", &"helloworld");//文字常量区
    printf("&main = %p\n", &main);//程序代码区
    printf("main = %p\n", main);
    printf("&kMax = %p\n", &kMax);
    printf("&kMin = %p\n", &kMin);

    printf("\n打印变量的值\n");
    printf("a = %d\n", a);
    printf("b = %d\n", b);

    delete pInt;
    pInt = nullptr;

    return 0;
}

