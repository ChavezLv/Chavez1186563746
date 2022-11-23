#include <math.h>
#include <iostream>
#include <set>
#include <vector>

using std::cout;
using std::endl;
using std::multiset;
using std::vector;

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
    /* int arr[10] = {1, 3, 6, 8, 9, 2, 5, 5, 5, 6}; */
    /* multiset<int, std::less<int>>number(arr, arr + 10); */
    /* multiset<int, std::greater<int>>number(arr, arr + 10); */
    multiset<int> number = {1, 3, 6, 8, 9, 2, 5, 5, 5, 6};
    display(number);

    //multiset的查找
    size_t cnt1 = number.count(12);
    size_t cnt2 = number.count(5);
    cout << "cnt1 = " << cnt1 << endl;
    cout << "cnt2 = " << cnt2 << endl;

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

    //multiset的插入
    auto it2= number.insert(100);
    if(it2 == number.end())
    {
        cout << "插入失败" << endl;
    }
    else
    {
        cout << *it2 << endl;
    }

    vector<int> vec = {8, 39, 29, 10};
    number.insert(vec.begin(), vec.end());
    display(number);

    /* std::initializer_list<int> il = {19, 39, 20, 10}; */
    number.insert(std::initializer_list<int>{19, 39, 20, 10});
    display(number);

    cout << endl << endl;
    auto it3 = number.begin();
    ++it3;
    number.erase(it3);
    display(number);

    // 插入
    cout << endl << "bound的测试" << endl;
    //区别: 比如 1 2 2 3 3 4 5 7 7 7 7 8 9 9 ....
    auto it4 = number.lower_bound(5);//lower_bound(7)返回第一个>= 给定的7的 第一个7
    auto it5 = number.upper_bound(5);//upper_bound(7)返回第一个>给定值的 第一个8 
    cout << "*it4  =" << *it4 << endl;
    cout << "*it5  =" << *it5 << endl;
    while(it4 != it5)
    {
        cout << *it4 << " ";
        ++it4;
    }//it4, it5中间会有间隔范围的
    cout << endl;

    // equal_range() 范围: 用法 = lower_bound + upper_bound
    auto ret = number.equal_range(5);
    while(ret.first != ret.second)
    {
        cout << *ret.first << "     ";
        ++ret.first;
    }
    cout << endl;
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
};//class pointer

std::ostream &operator<<(std::ostream &os, const Point &rhs)
{
    os << "(" << rhs._ix 
      << "," << rhs._iy
      << ")";
    return os;
}
bool operator<(const Point &lhs, const Point &rhs)//重载<，【并非某个成员函数】，重载的就是【全局函数】
{//这个【重载】，通过【看参数的类型】，就可以辨识出【应该用这个】
//其实【不以成员函数重写】，最大的影响就是【拿内部数据不方便】，所以本函数都是【以接口的形式获取点数据】，是可以的
    cout << "bool operator<(const Point &, const Point &)" << endl;
    if(lhs.getDistance() < rhs.getDistance())
    {
        return true; //距离小于是true没错
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

bool operator>(const Point &lhs, const Point &rhs) //传的是greater应该就是来访问它
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

struct Comparetion //传一个【重载过函数对象的】类
{
    bool operator()(const Point &lhs, const Point &rhs) const //左操作数和右操作数
    {//默认是小于号？你想是什么号是什么号？--->答案应该是后者，就是【取就行】
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
    /* multiset<Point> number = { */
    /* multiset<Point, std::less<Point>> number = { *///填这个的话就会去拿小于号
    /* multiset<Point, std::greater<Point>> number = { */
    multiset<Point, Comparetion> number = { //填类模板参数
        Point(1, 2),
        Point(1, -2),
        Point(-1, 2),
        Point(-1, 2),
        Point(10, 2),
        Point(0, 2),
        Point(0, 2),
        Point(-3, 2),
        Point(1, 2),
    };
    display(number);
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

