#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using std::cout;
using std::endl;
using std::for_each;
using std::vector;
using std::ostream_iterator;
using std::copy;
using std::count;
using std::find;

//course1 
// 笔记by lcl:

/*
算法只讲下容易出错误的

Defined in header <algorithm>
(1) 
template< class InputIt, class UnaryFunction >
UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
(until C++20)
template< class InputIt, class UnaryFunction >
constexpr UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
(since C++20)
template< class ExecutionPolicy, class ForwardIt, class UnaryFunction2 >
void for_each( ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryFunction2 f );


a)非成员函数, 只能从头到尾传入参数


b)
一元函数 : 函数的参数只有一个 如UnaryFunction f


一元断言/谓词 : 函数参数是一个, 函数的返回类型是bool
比如remove_if里面 Unary


c)  现在用vector创建对象
回顾遍历方式: ij/  ... /输出流迭代器

//遍历方式: copy(number.begin(), number.end(), ostream_iterator<int> (cout, "\n");
    遍历方式: for_each(number.begin(), number.end(), print); 

for_each(), 从头到尾一个个传入一元函数print(int &){}

d)因为for_each是非修改式算法, 但是想要改数据, 只能在 print这里动下手脚

如下 多个 引用&

所以说for_each是非修改式算法并不是很准确

d)类似之前的, 匿名函数, 使用c++11里面的 lambda表达式, 匿名函数
解释: []表示匿名函数  大括号表示函数体
好处: 不在同一个文件时, 不好看实现, 所以用lambda表达式可以提高程序的\
的可读行, 更加直观

注意: 这个函数体的内容不要写得太多, 类似inline的小巧即可


-------------------count---------------------
Defined in header <algorithm>
(1) 
template< class InputIt, class T >
typename iterator_traits<InputIt>::difference_type
    count( InputIt first, InputIt last, const T &value );\

    解释: difference_type这里也是类型萃取,不针对具体迭代器

size_t cnt1 = count(number.begin(), number.end(), 3);//看数字3在范围内出现了几次

--------------------find--------------------

Defined in header <algorithm>
(1) 
template< class InputIt, class T >
InputIt find( InputIt first, InputIt last, const T& value );

同样是返回一个迭代器
auto it = find(number.begin(), number.end(), 7);

易错: 必须要判断一下返回的迭代器是否等于end(), 否则去使用(比如解引用)
就有问题了!!!


------------------binary_search----------------------------

auto ret = binary_search(nums.begin(), nums.end(), 3);
//ret = 1

注意: 没有顺序不能进行二分查找, 所以使用之前必须排序

还有很多......
----------比如lower_bound, equal_range讲过的复习一下  77778


--------------接下来-----remove_if-------

*/




void print(int &val)
{
    ++val;
    cout << val << "  ";//没有引用的话, 只是显示+1
}
void test()
{
    vector<int> number = {1, 4, 7, 9, 3, 2};
    //遍历方式: copy(number.begin(), number.end(), ostream_iterator<int> (cout, "\n");
    /*遍历方式: for_each(number.begin(), number.end(), print); */
    //lambda表达式，匿名函数，C++11提出来的
    for_each(number.begin(), number.end(), [](int &val){
             ++val;
             cout << val << "  ";
             });
    cout << endl;
    copy(number.begin(), number.end(), ostream_iterator<int>(cout, "  "));
    cout << endl;


    cout << endl;
    size_t cnt1 = count(number.begin(), number.end(), 3);
    cout << "cnt1 = " << cnt1 << endl;

    auto it = find(number.begin(), number.end(), 7);
    if(it == number.end())
    {
        cout << "该元素不存在vector中" << endl;
    }
    else
    {
        cout << *it << "  ";
    }
    cout << endl;

    sort(number.begin(), number.end());
    copy(number.begin(), number.end(), ostream_iterator<int>(cout, "  "));
    cout << endl;
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

