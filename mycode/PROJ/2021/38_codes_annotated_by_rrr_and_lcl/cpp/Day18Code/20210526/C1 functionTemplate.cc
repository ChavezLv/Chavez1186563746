#include <string.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

          //参数类型推导
//函数模板================= 模板函数
//         实例化
//实例化：显示实例化和隐式实例化
//函数模板
/* template <typename T> */
template <class T>//模板参数列表
T add(T x, T y)
{
    cout << "T add(T, T)" << endl;
    return x + y;
}

template <class T>//模板参数列表
T add(T x, T y, T z)
{
    cout << "T add(T, T, T)" << endl;
    return x + y + z;
}

#if 0
int add(int x, int y)
{
    cout << "int add(int, int)" << endl;
    return x + y;
}

double add(double x, double y)
{
    cout << "double add(double , double)" << endl;
    return x + y;
}

float add(float x, float y)
{
    return x + y;
}
#endif

//模板的特化(模板的特殊形式)
//模板的全特化(模板的特殊形式全部写出来)和偏特化(部分特化)\
现在也是全特化:template <class T>, 只有一个参数,算是都写出来了

//函数模板只支持全特化，不支持偏特化(c++11之前, c++11之后也支持了)
//类模板既支持全特化，也支持偏特化
//对于函数模板, 关注全特化即可, 对于类模板, 关注偏特化即可

//比如template<class T1, class T2> 有两个参数, \
比如个数的形式全部写出来就是template<int, char>
template <>//可以不写, 只是为了表明是模板的特化, 也是推导出来, 本来\
                add(pstr1, pstr2)是错误的, 现在也能用了
const char *add(const char *px, const char *py)
{
    cout << "const char *add(const char *, const char *)" << endl;
    size_t len =  strlen(px) + strlen(py) + 1;
    char *pTemp = (char *) malloc(len);
    memset(pTemp, 0, len);
    strcpy(pTemp, px);
    strcat(pTemp, py);


    return pTemp;
    
}
//course1 
// 笔记by lcl:\
1)比如传进去的是两个int, 就会去推导, 把T推导为int, 这个推导过程可能会降低效率\
思考: 如果显示实例化呢? 就不用推导了\
\
2) 比如函数模板T add(T x, T y)和普通函数int add(int x, int y)同时存在时, \
会优先调用普通函数\
  由上可知, 普通函数于函数模板之间是可以进行重载的\
3)T add(T x, T y, T z) 与 T add(T x, T y)\
可知函数模板与函数模板之间也是可以进行重载的\
4) 使用cout << "add(ia, db) = " << add(ia, db) << endl;\
除非有写具体类型函数int add(int x, int y)能隐式转换的情况下不会错, 没有写时\
会出错, 因为首先ia推导为int, 所有的T都变为int了, db再推导时就有二义性了

//5)\
函数模板  vs 模板函数(已经推导过来了, 就是一个普通函数)\
由抽象类型的函数模板  通过模板参数推导 为模板函数\
就是抽象到具体, 也称为实例化\
类到对象   也是抽象到具体\
//6)实例化分类\
add<double>(da, db) << endl;//显示实例化\
add(ia, ib) << endl;//隐式实例化 编译器自己推导\

//7)接下来 分成声明和实现\
看demo文件夹: 结论-- 不能分开声明与实现, 类比inline\
\
8) 模板的特化\
cout << "add(pstr1, pstr2) = " << add(pstr1, pstr2) << endl;当没有特化会出错\
\
const char *add(const char *px, const char *py){}\
模板的特化(模板的特殊形式)\
现在也是全特化:template <class T>, 只有一个参数,算是都写出来了\
\
8.1) 模板的全特化(模板的特殊形式全部写出来)和偏特化(部分特化)\
其余笔记在上面有


//9) 接下来看下模板的参数类型(要记忆!)\
看templatePar.cc 即可

//10) course 2了

//注意: 偏特化和全特化  使用时候的具体情况还要再试一下

void test()
{
    int ia = 3, ib = 4;
    double da = 3.3, db = 4.4;
    char ca = 'a', cb = 1;

    const char *pstr1 = "hello";
    const char *pstr2 = "world";

    cout << "add(ia, ib) = " << add(ia, ib) << endl;//隐式实例化
    cout << "add(da, db) = " << add<double>(da, db) << endl;//显示实例化
    cout << "add(ca, cb) = " << add(ca, cb) << endl;
    /* cout << "add(ia, db) = " << add(ia, db) << endl;//error,ia会推导T为int，db会推导T为double，所以就矛盾 */

    cout << "add(pstr1, pstr2) = " << add(pstr1, pstr2) << endl;
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

