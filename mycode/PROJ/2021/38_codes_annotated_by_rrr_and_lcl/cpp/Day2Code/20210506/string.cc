#include <string.h>//C的头文件
#include <iostream>
#include <string>//C++头文件

using std::cout;
using std::endl;
using std::string;

void test()
{
    //char str1[] = {'h', 'e', 'l', 'l', 'o'};
    //C风格字符串默认以'\0'结尾
    char  str1[] = "hello";
    char str2[] = "world";//c风格字符串
    str1[0] = 'H';//const *p1
    /* str1 = nullptr;//error, * const */ 
    printf("str1 = %s\n", str1);

    const char *pstr = "hello, world";//常量字符串
    /* printf("111\n"); */
    /* pstr[0] = 'H';//error */
    /* printf("222\n"); */
    pstr = nullptr;
    /* printf("333\n"); */
    
    /* size_t len2 = sizeof(pstr);//8 */
    /* size_t len3 = strlen(pstr); */
    size_t len1 = sizeof(str1);
    size_t len2 = sizeof(str2); 
    printf("len1 = %lu\n", len1);
    printf("len2 = %lu\n", len2);

    size_t len = len1 + len2 - 1;
    char *pstr2 = static_cast<char *>(malloc(len)); //强转为char*类型管道空间
    memset(pstr2, 0, sizeof(len));
    strcpy(pstr2, str1);
    strcat(pstr2, str2);
    printf("pstr2 = %s\n", pstr2);

    free(pstr2);
    pstr2 = nullptr;
}
#if 0
typedef struct A
{
int ia;
char cb;
}structAA;

structAA a;
a.ia = 10;
a.cb = 'c';
#endif

void test2()
{
    //C++        C
    string s1 = "hello";//C风格字符串可以转换为C++风格字符串
    string s2 = "world";
    string s3 = s1 + s2;//Cpp 的str可以快速的完成连接操作
    cout << "s1 = " << s1 << endl
         << "s2 = " << s2 << endl
         << "s3 = " << s3 << endl;

    //从C++风格字符串转换为C风格字符串
    cout << endl;
    const char *pstr = s3.c_str();
    cout << "pstr = " << pstr << endl;

    //C++风格字符串的长度
    cout << endl;
    /* size_t len1 = sizeof(s3);//与string的底层实现有关 */
    size_t len2 = s3.size();//这两个在str含义是一样的
    size_t len3 = s3.length();
    /* cout << "len1 = " << len1 << endl; */
    cout << "len2 = " << len2 << endl;
    cout << "len3 = " << len3 << endl;

    //遍历C++风格字符串
    cout << endl;
    for(size_t idx = 0; idx != len2; ++idx)
    {
        cout << s3[idx] << "   ";//直接打印字符串
    }
    cout << endl;

    //C++风格字符串的拼接
    cout << endl;
    string s4 = s3 + "wuhan";//cpp风格字符串可以直接拼接c风格字符串
    cout << "s4 = " << s4 << endl;

    cout << endl;
    string s5 = s4 + 'A';
    cout << "s5 = " << s5 << endl;

    cout << endl;
    s5.append(s1);//也可以调用函数拼接字符串
    cout << "s5 = " << s5 << endl;

}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

