#include <iostream>
#include <vector>
#include <deque>
#include <list>

using std::cout;
using std::endl;
using std::vector;
using std::deque;
using std::list;
//模板: 
template <typename Container>
void display(const Container &c)
{
    for(auto &elem : c)
    {
        cout << elem << "  ";
    }
    cout << endl;
}
//course3
// 笔记by lcl:\
一) 下午:deque 
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class deque : protected _Deque_base<_Tp, _Alloc>
    {

    }
    //基类_Deque_base
    template<typename _Tp, typename _Alloc>
    class _Deque_base
    {

    }

// 1) allocator_type的源码, 常见写法
    typedef _Alloc        allocator_type;
    // 老师源码版本: typedef typename _Base::allocator_type allocator_type;
// 2) 然后看到deque原理图解(STL相关图解.pptx)\
    存储:  逻辑联系但是物理分散( 有一个中控器数组Map(类似于电脑的CPU) )\
    特点:  小片段内部是连续的, 片段之间是不连续的(看图示很明显, 图案有点类似于哈希冲突的链表解决方案)
// 3) 主要是学习vector和deque的实现区别, list是双向链表之前作业有\
     写过, 就不讲了

// 二) 继续学习 vector的插入 \
     insert  先让it指到中间位置(在中间位置插入)\
     1) number.insert(it, 100); //插入前后迭代器的pos位置不变, pos之后所有元素后移一格\
     2) number.insert(it, arr, arr + 5);//   迭代器范围范围插入\
     3) number.insert(it, 20, 200);// 20个200插入
     //这里容易发生core_dumped, 为什么? \
     因为插入前后it 的位置不会改变, 但是当插入操作可能会触发扩容\
     扩容之后, 之前的迭代器就失效了, 这时候还用旧的迭代器就可能触发bug\
     因为每次插入之后, 都要重新置位it = nums.begin(), 这是一个好习惯!   \
     \
     4)查看deque的 insert, 先判断插入位置是前一半push_front()还是后一半(push_back(back()), 是后一半就换个地方插入?\
     ...\
     由上可知, vector/deque的insert效率并不高, 一半不建议用这个函数,\
     所以提供了另外的头尾插入

     //\
     erase   \
     1) vector的erase特点: 数据连续的容器, 每次在中间erase之后, 就会多出一个空位\
        因此要注意, 比如使用erase想删除所有为6的元素, 要注意遍历时\
        \
        易错点改正: ++语句不能放到for里面,  删除时不++, 没有删除时再++
//course4
// 笔记by lcl:\
//接下到第四节课: course4
// 一) 容器元素的清空\
1) vector用size()和capacity来看下清空前后的区别\
nums.clear();// C4: 元素清空, size为0, 但是容量没有为0, 需要shrink_to_fit\
nums.shrink_to_fit();//缩减内存(注意: 只是回收没有用的空间). 容量也会变为0\
\
2) deque\
deque没有capacity函数\
\
3) list\
没有capacity, 也没有shrink_to_fit()? \
因为之前写过list, 删除时就回收了, 所以size就是当前, 不需要capacity
// 二) 其他共同函数\
1) emplace_back/emplace_front  C++11 \
比如
    vector<Point> number;
    number.push_back(Point(1, 2));
    number.push_back({1, 2});//思考下过程, 比如push_back的移动构造什么的
    number.emplace_back(1, 2);//可以传多个参数 模板参数包-->函数参数包. \
    替代push_back, 减少一次临时对象的创建
    //1作为横坐标, 2作为纵坐标
//  vector/deque/list也有同样的接口用法

// 三) list的独有函数\  --->> list.cc
// ....\
\
\
\
//笔记在list.cc  
//之后Course5


// 四) [不需要死记]insert的扩容机制: 知道不是每次都是两倍就行 
/*
push_back可以进行两倍扩容, 是因为每次只插入一个元素, 

经过测试可知: 
    insert的插入元素是不确定的, 
    size() = n, capacity() = m, insert 元素个数为 t

    1).  if   t <= m - n --> new m = m(不扩容)
    2)   if   m - n < t, t < n --> new m = 2 * n;
    3)   if   m - n < t, n < t < m --> new m = n + t;
    4)   if   m - n < t, n > m --> new m = n + t
    
*/


// 下一天内容, [问:] 哪些容器? \
    stack queue  priority_queue不能用迭代器遍历\
    实际上这些是适配器\
    \
    还有set, 自定义类型如何使用且排序

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }
private:
    int _ix;
    int _iy;
};
void test00()
{
    vector<Point> number;
    number.push_back(Point(1, 2));
    number.push_back({1, 2});//思考下过程, 比如push_back的移动构造什么的
    number.emplace_back(1, 2);//可以传多个参数 模板参数包-->函数参数包. \
    替代push_back, 减少一次临时对象的创建
    //1作为横坐标, 2作为纵坐标
}
void test()
{   //开头, capacity, size,三个指针 所以都是24!
    cout << "sizeof(vector<int>) = " << sizeof(vector<int>) << endl;//24, 三个指针
    cout << "sizeof(vector<double>) = " << sizeof(vector<double>) << endl << endl;
    cout << "sizeof(vector<char>) = " << sizeof(vector<char>) << endl << endl;

    vector<int> number = {1, 3, 5, 9, 8, 7, 5, 3, 2, 4};
    display(number);
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    cout << endl << "在vector的尾部进行插入与删除" << endl;
    number.push_back(12);
    number.push_back(6);
    display(number);
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;
    number.shrink_to_fit();
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;
    number.pop_back();
    display(number);
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    &number;//获取不到第一个元素的地址，只能获取_M_start的地址
    &number[0];//获取第一个元素的地址
    &*number.begin();//ok
    //vector为何不支持在头部进行插入与删除？时间复杂度O(N)
    

    cout << endl << "在vector中间去进行插入" << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;
    auto it = number.begin();//
    ++it;//
    ++it;//
    cout << "*it = " << *it << endl;
    number.insert(it, 100);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    cout << endl;
    cout << "*it = " << *it << endl;
    number.insert(it, 20, 200);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    cout << endl;
    int arr[5] = {1, 6, 9, 8, 7};
    it = number.begin();
    ++it;
    ++it;
    cout << "*it = " << *it << endl;
    number.insert(it, arr, arr + 5);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    cout << endl << "vector清空元素" << endl;
    number.clear();// C4: 元素清空, size为0, 但是容量没有为0, 需要shrink_to_fit
    number.shrink_to_fit();//缩减内存(注意: 只是回收没有用的空间). 容量也会变为0,
    cout << "number.size() = " << number.size() << endl;//
    cout << "number.capacity() = " << number.capacity() <<endl;

}

void test2()
{
    deque<int> number = {1, 3, 5, 9, 8, 7, 5, 3, 2, 4};
    display(number);
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "在deque的尾部进行插入与删除" << endl;
    number.push_back(12);
    number.push_back(6);
    display(number);
    cout << "number.size() = " << number.size() << endl;
    number.pop_back();
    display(number);
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "在deque的头部进行插入与删除" << endl;
    number.push_front(34);
    number.push_front(10);
    display(number);
    cout << "number.size() = " << number.size() << endl;
    number.pop_front();
    display(number);
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "在deque中间去进行插入" << endl;
    auto it = number.begin();
    ++it;
    ++it;
    cout << "*it = " << *it << endl;
    number.insert(it, 100);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;

    cout << endl;
    cout << "*it = " << *it << endl;
    number.insert(it, 3, 200);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;

    cout << endl;
    int arr[5] = {1, 6, 9, 8, 7};
    cout << "*it = " << *it << endl;
    number.insert(it, arr, arr + 5);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "deque清空元素" << endl;
    number.clear();
    number.shrink_to_fit();
    cout << "number.size() = " << number.size() << endl;
}

void test3()
{
    list<int> number = {1, 3, 5, 9, 8, 7, 5, 3, 2, 4};
    display(number);
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "在list的尾部进行插入与删除" << endl;//尾插法
    number.push_back(12);
    number.push_back(6);
    display(number);
    cout << "number.size() = " << number.size() << endl;
    number.pop_back();
    display(number);
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "在list的头部进行插入与删除" << endl;//头插法
    number.push_front(20);
    number.push_front(0);
    display(number);
    cout << "number.size() = " << number.size() << endl;
    number.pop_front();
    display(number);
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "在list中间去进行插入" << endl;
    auto it = number.begin();
    ++it;
    ++it;
    cout << "*it = " << *it << endl;
    number.insert(it, 100);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;

    cout << endl;
    cout << "*it = " << *it << endl;
    number.insert(it, 3, 200);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;

    cout << endl;
    int arr[5] = {1, 6, 9, 8, 7};
    cout << "*it = " << *it << endl;
    number.insert(it, arr, arr + 5);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;

    cout << endl << "list清空元素" << endl;
    number.clear();
    cout << "number.size() = " << number.size() << endl;


}
int main(int argc, char **argv)
{
    test();

    cout << endl << "测试deque" <<endl << endl;
    test2();

    cout << endl << "测试list" <<endl << endl;
    test3();
    return 0;
}

