#include <math.h>
#include <iostream>
#include <set>
#include <vector>

using std::cout;
using std::endl;
using std::set;
using std::vector;

//course2 
// 笔记by lcl:\
0) #include <set> set和multi_set \
   #include <map> map和multi_map 
template<
    class Key,
    class Compare = std::less<Key>, //默认从小到大的顺序排列
    class Allocator = std::allocator<Key>
> class set;
// 复习: 之前学习的set特点:\
关键字唯一(存的数据, 只存一个数据), 默认升序, 底层红黑树
//set和multi_set 只比vector少一个const的构造方式
// 一) 构造, 查找, 插入, 删除\
set<int, std::greater<int>>number(arr, arr + 10); //从大->小

// 二) set存自定义类型Point
//因为set是排序的, 所以Point没有对小于符号进行重载时, 编译就会报错

//技巧: 当报错很多的时候, 只看红色字体的东西即可.

// 接下来看一下multiset  唯一区别就是key值是可以相同的\
这就导致了, insert的返回值不一样, 因为key可以相同, 只要插入的相同的类型\
就必定可以成功
    //multiset的插入
//  auto it2= number.insert(100);
//  if(it2 == number.end())  // 不用判断的, 必拿下
// 三) multiset\
equal_range() 范围: 用法 = lower_bound + upper_bound

//------------------------以上都是笔记--------------------

template <typename Container>
void display(const Container &c)
{
    for(auto &elem : c)
    {
        cout << elem << "  ";
    }
    cout << endl;
}

void test()
{
    //set的构造
    /* int arr[10] = {1, 3, 6, 8, 9, 2, 5, 5, 5, 6}; */
    /* set<int, std::less<int>>number(arr, arr + 10); //从小->大 */
    /* set<int, std::greater<int>>number(arr, arr + 10); //从大->小 */
    set<int, std::greater<int>> number = {1, 3, 6, 8, 9, 2, 5, 5, 5, 6};
    display(number);

    //set的查找(1)  count返回值不是1,就是0
    size_t cnt1 = number.count(12);
    size_t cnt2 = number.count(9);
    cout << "cnt1 = " << cnt1 << endl;
    cout << "cnt2 = " << cnt2 << endl;

    //set的查找(2)  find返回一个迭代器, 比较是否为空的方式\
    不是随意使用的, 要显得专业. number.end() 最后一个元素的下一个\
    位置, 不要用nullptr和find返回的迭代器进行比较, \
    类类型的指针就不好了, 比如deque的时候就是\
    类类型的指针
    cout << endl;
    auto it = number.find(9);
    if(it == number.end())
    {
        cout << "该元素不存在" << endl;
    }
    else
    {
        cout << "*it = " << *it <<endl;
    }

    //set的插入 insert 插入迭代器范围的
    std::pair<set<int>::iterator, bool> ret = number.insert(100);
    if(ret.second)
    {
        cout << *ret.first << endl;
    }
    else
    {
        cout << "插入失败" << endl;
    }
    //set的插入形式(1) : insert 插入迭代器范围的, 此时返回值是一个void
    vector<int> vec = {8, 39, 29, 10};// 这个现在是动态数组的可以换成普通数组
    number.insert(vec.begin(), vec.end());//左闭右开
    display(number);// 插入排序
    
    //set的插入形式(2) : \
    void insert( std::initializer_list<value_type> ilist );// 类<类型> 对象(可以时匿名对象或者有名对象)
    /* std::initializer_list<int> il = {19, 39, 20, 10}; */
    //number.insert(std::initializer_list<int>({19, 39, 20, 10}));//ok
    number.insert(std::initializer_list<int>{19, 39, 20, 10});
    display(number);



    //set的删除: erase
    //因为底层用的是红黑树, 删除操作这里的具体实现不用管它.
    //也因为这样, 所以不允许使用[]访问运算符, 除非是增加或者删除
    cout << endl << endl;
    auto it2 = number.begin();
    ++it2;
    ++it2;
    cout << *it2 << endl;
    number.erase(it2);
    display(number);
}

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        /* cout << "Point(int = 0, int = 0)" << endl; */
    }

    double getDistance() const
    {
        return hypot(_ix, _iy);
    }

    int getX() const 
    {
        return _ix;
    }

    int getY() const
    {
        return _iy;
    }

    ~Point()
    {
        /* cout << "~Point()" << endl; */
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &rhs);
private:
    int _ix;
    int _iy;
};

std::ostream &operator<<(std::ostream &os, const Point &rhs)
{
    os << "(" << rhs._ix 
      << "," << rhs._iy
      << ")";
    return os;
}

//std::less
// 因为有getX getY 所以不用作为友元的形式
bool operator<(const Point &lhs, const Point &rhs)
{
    cout << "bool operator<(const Point &, const Point &)" << endl;
    if(lhs.getDistance() < rhs.getDistance())
    {
        return true;
    }
    else if(lhs.getDistance() == rhs.getDistance())
    {
        if(lhs.getX() < rhs.getX())
        {
            return true;
        }
        else if(lhs.getX() == rhs.getX())
        {
            if(lhs.getY() < rhs.getY())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }
    else
    {
        return false;
    }
}
//std::greater
bool operator>(const Point &lhs, const Point &rhs)
{
    cout << "bool operator>(const Point &, const Point &)" << endl;
    if(lhs.getDistance() > rhs.getDistance())
    {
        return true;
    }
    else if(lhs.getDistance() == rhs.getDistance())
    {
        if(lhs.getX() > rhs.getX())
        {
            return true;
        }
        else if(lhs.getX() == rhs.getX())
        {
            if(lhs.getY() > rhs.getY())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }
    else
    {
        return false;
    }
}

//思考: 有必要作为友元的形式吗?
struct Comparetion
{
    bool operator()(const Point &lhs, const Point &rhs) const
    {
        cout << "struct Comparetion" << endl;
         if(lhs.getDistance() < rhs.getDistance())
         {
             return true;
         }
         else if(lhs.getDistance() == rhs.getDistance())
         {
             if(lhs.getX() < rhs.getX())
             {
                 return true;
             }
             else if(lhs.getX() == rhs.getX())
             {
                 if(lhs.getY() < rhs.getY())
                 {
                     return true;
                 }
                 else
                 {
                     return false;
                 }
             }

         }
         else
         {
             return false;
         }
    }
};
void test2()
{
    /* set<Point> number = { // 默认使less*/
    /* set<Point, std::less<Point>> number = { //重载<之后插入就不会报错*/
    /* set<Point, std::greater<Point>> number = { //重载>之后插入就不会报错*/
    set<Point, Comparetion> number = {
        Point(1, 2),
        Point(1, -2),
        Point(-1, 2),
        Point(10, 2),
        Point(0, 2),
        Point(-3, 2),
    };
    display(number);
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

