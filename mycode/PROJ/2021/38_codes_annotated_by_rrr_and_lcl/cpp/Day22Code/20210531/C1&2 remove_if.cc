#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using std::cout;
using std::endl;
using std::vector;
using std::ostream_iterator;
using std::copy;
using std::count;
using std::find;


//course1 
// 笔记by lcl:
/*
-----------------remove_if + erase----------------------------


template< class ForwardIt, class UnaryPredicate >
ForwardIt remove_if( ForwardIt first, ForwardIt last, UnaryPredicate p );
0) 传参: 从头到尾, 一言断言

1)同样的也是

bool func(int val)
{
    return val > 5; //如果大于5, func返回true, 全删掉
}

auto it = remove_if(number.begin(), number.end(), func); 
// 元素有多个大于if的, 个数没有变, 只变了一个, 错误

为什么? 
2) 看下cppreference的实现

1, 3, 7, 9, 4, 3, 6, 5, 2

template<class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
    first = std::find_if(first, last, p);
    if (first != last)
        for(ForwardIt i = first; ++i != last; )
            if (!p(*i))
                *first++ = std::move(*i);
    return first;
}
-------------------分析: ------------------
template<class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
    first = std::find_if(first, last, p);//查找第一个满足条件p的迭代器, 
    //第一个就是p(7)返回true时候的返回
    if (first != last)
    {
        // first 赋值给 i
         for(ForwardIt i = first; ++i != last; )
         {
             if (!p(*i))// p(7)取反进不去,然后再++
             {
                  *first++ = std::move(*i);// 内置类型std::move无效果
                 // 目的: 加快效率, 比如当非内置类型时
                 
                 //下一个是4, 赋值给7的位置, 所以7变成了4. f++ 
             }  
         }   
    } 
    return first;
}
1, 3, 5, 7, 4, 3, 2, 6, 8, 9, 1
    
                     f           l
1, 3, 5, 4, 3, 2, 1, 6, 8, 9, 1
                                 i  (i!=last了, 返回first, 待删除元素的首地址), 所以就这样打印出来了)
    
    
template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first) 
    {
        if (p(*first)) {// 第一次返回false, 因为是1 , 直到  p(7)返回true, 
            return first;// p(7) 返回 7
        }
    }
    return last;
}

----------------结论: 要接收返回值, 再用erase------------------------------
auto it = remove_if(number.begin(), number.end(), bind2nd(std::greater<int>(), 5));
    number.erase(it, number.end());

3)为什么要这么设计? 整得这么麻烦!

原因:  序列式容器, 如7, vector, 删7, 往前挪, 这样就会漏掉某些元素
        之前有讲过! 
漏掉: 就是我++, 你后挪--, 这样就擦肩而过了

        如别的list, set 就不是连续的, 就没有关系

        只是适应元素相邻的的容器的算法, 更具有通用性, 泛型算法! yyds


4) 思考: 对于容器, 如果进行修改(写操作). 不要边写边读

比如删除->挪动->再读--> 就是脏数据

比如之前进程,线程的时候

举例: ---------> test2(){}

和之前insert的时候, 也是一样的, 好习惯是, 每次写操作要置位迭代器!


5)----------------bind1st, 
auto it = remove_if(number.begin(), number.end(), bind1st(std::less<int>(), 5));
二元函数的一次个参数已经固定死了, 第二个参数就是从begin()到end()
bind1st(F, T);// F是一个二元函数, value > 5

std::less<int>  les;
然后写的时候 std::bind1st(les, 5);
// 函数的名字, 可以写成有名的形式, 也可以写成匿名的形式



二元函数
bind
f  x

第一个参数f用x去固定


6)------------------------bind2nd

 C++ Utilities library Function objects

固定二元函数的第二个参数:
bind1st(F, 5)
F(5, number.begin(), number.end())
std::less(5, Y)  // 5 < Y
    
    
bind2nd(F, 5)     
F(number.begin(), number.end(), 5)// 第二个参数用5固定了, 
    //number.begin(), number.end()看成整体
std::greater(Y, 5)// Y > 5


总结一下: 
remove_if( , , ) 参3就是要一个一言断言, bool f( 1个参)  

所以bind1st/ bind2nd就可以 把二元函数less的第1/2个参数固定

less和greater的返回类型就是bool, 刚好符合

另外, less和greater是template< class T > struct less;这样的函数对象, 别忘了
file:///C:/Users/Charlie%20Lee/Desktop/%E6%89%8B%E5%86%8C%E7%B1%BB/cpp_reference/reference/en/cpp/utility/functional/less.html


constexpr bool operator()(const T &lhs, const T &rhs) const 
{
    return lhs < rhs;
}


7)  -----------------bind
Defined in header <functional>
template< class F, class... Args >
 bind( F&& f, Args&&... args );
//unspecified
用法有点像 pair(两个参数)
可以传多个参数, 可变, bind前面没有看到返回类型, 就可以让auto去
类型推导, 因为后面一点才讲到.


---------------例子:------------------



*/


bool func(int val)
{
    return val > 5; //如果大于5, func返回true, 全删掉
}
void test()
{
    vector<int> number = {1, 3, 7, 9, 4, 3, 6, 5, 2};
    copy(number.begin(), number.end(), ostream_iterator<int>(cout, "  "));
    cout << endl;

    /* auto it = remove_if(number.begin(), number.end(), func); */
    /* auto it = remove_if(number.begin(), number.end(), [](int val){ */
    /*                     return val > 5; */
    /*                     }); */

    /* auto it = remove_if(number.begin(), number.end(), bind1st(std::less<int>(), 5)); */
    auto it = remove_if(number.begin(), number.end(), bind2nd(std::greater<int>(), 5));
    number.erase(it, number.end());
    copy(number.begin(), number.end(), ostream_iterator<int>(cout, "  "));
    cout << endl;
}

void test2()
{
    vector<int> number;
    number.push_back(1);

    bool flag = true;
    for(auto it = number.begin(); it != number.end(); ++it)
    {
        cout << *it << "  ";
        if(flag)
        {
            number.push_back(2);//读的时候进行了 写操作. 底层已经发生了扩容
            flag = false;
            it = number.begin();// 如果没有重置一次it就会出问题 , 看几率也能找到2, 中间打印n个随机数
            /* ++it; */
        }
        cout << endl;
    }
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

