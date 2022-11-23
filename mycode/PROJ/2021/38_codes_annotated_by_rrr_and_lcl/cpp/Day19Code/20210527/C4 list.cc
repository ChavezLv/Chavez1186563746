#include <iostream>
#include <list>

using std::cout;
using std::endl;
using std::list;

template <typename Container>
void display(const Container &c)
{
    for(auto &elem : c)
    {
        cout << elem << "  ";
    }
    cout << endl;
}
#if 1
template <typename T>
struct Comparetion
{
    bool operator()(const T &lhs, const T &rhs) const
    {
        cout << "bool operator()(const T &, const T &)" << endl;
        return lhs < rhs;
    }
};
#endif
#if 0
bool operator<(const list &lhs, const list &rhs)
{
    cout << "bool operator<(const list<int> &, const list <int>) " << endl;
    return lhs < rhs;
}
#endif

//course4
// 笔记by lcl:\
// 三) list 的独有函数\  --->> list.cc
// 1) sort(默认从小到大)
        nums.sort();
// 2) sort  的 less 和 greater
        number.sort(std::less<int>()); //从小到大*/
        number.sort(std::greater<int>()); // 从大到小*/
// 3) 自己重载(函数对象) 也叫仿函数\
    重载了函数调用运算符的类，在其调用运算符执行时，可以称作函数对象
        number.sort(Comparetion<int>());// 传入一个类的对象\
        形式和之前学习的删除器shared_ptr的传参是类似的
// 4) nums.reverse()
        nums.reverse();  
// 5) nums.unique();  // 使用unique要先排序!!!

// 6) merge() 要先排序, 传入另外一个链表, merge结束之后, 另外一个链表\
就没有了, 打印时就是空的了

// 7) splice()  \
number.splice(it, number3);//挪动number3的所有元素到number \
number.splice(it, number3, it2);// 挪动number3的单个指定元素到number  \
number.splice(it, number3, it2, it3);// 挪动number3的it2->it3范围元素到number\
问: 上面的3种用法适用于自己挪动自己吗?  \
可以的. 挪动单个..., 多个number.splice(it4, number, it5, it6);\
但是注意挪动多个的时候, 需要避免操作范围的交叉, 类似于strcpy和strmove

// 8)还剩一个rm后面将....

//下一节课Course5, 讲insert的源码和扩容机制

void test()
{
    list<int> number = {1, 3, 5, 7, 1, 3, 8, 2, 4, 6, 3};
    display(number);
    number.unique();//uniqe去重的时候，保证元素是有序的
    display(number);

    cout << endl << "测试sort函数" << endl;
    /* number.sort(std::less<int>()); //从小到大*/
    /* number.sort(std::greater<int>()); // 从大到小*/
    /* number.sort(Comparetion<int>()); */
    number.sort();
    display(number);

    cout << endl << "链表的逆置reverse" << endl;
    number.reverse();
    display(number);

    //链表合并的时候，也要保证链表有序
    cout << endl << "链表的合并merge" << endl;
    number.sort(std::less<int>());
    display(number);
    list<int> number2 = {10, 40, 30, 5};
    number2.sort();// 要先排序
    display(number2);
    number.merge(number2);
    display(number);
    display(number2);//数据没有了

    cout << endl << "去除重复元素unique" << endl;
    number.unique();
    display(number);


    cout << endl << "移动元素splice" << endl;
    list<int> number3 = {5, 8, 19, 30, 4, 7};
    display(number3);
    auto it = number.begin();
    ++it;
    ++it;
    /* number.splice(it, number3); */
    // 这样就是把numbers3所有的元素移到it前面去
    
    /* display(number); */
    /* display(number3);//splice之后元素都没有了 */

    auto it2 = number3.begin();
    ++it2;
    ++it2;
    number.splice(it, number3, it2);// 挪动单个元素
    display(number);
    display(number3);

    cout << endl << endl;
    it2 = number3.begin();
    auto it3 = number3.begin();
    ++it3;
    ++it3;
    cout << "*it = " << *it <<endl;
    number.splice(it, number3, it2, it3);
    display(number);
    display(number3);

    cout << endl << "在同一个链表中进行splice" << endl;
    auto it4 = number.begin();
    auto it5 = number.end();
    auto it6 = number.end();
    ++it4;
    ++it4;
    --it5;
    --it5;
    --it5;
    --it6;
    cout << "it4 = " << *it4 << endl;
    cout << "it5 = " << *it5 << endl;
    cout << "it6 = " << *it6 << endl;
    number.splice(it4, number, it5, it6);
    display(number);
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

