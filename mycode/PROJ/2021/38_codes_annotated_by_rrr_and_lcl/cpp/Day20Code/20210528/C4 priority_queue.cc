#include <math.h>
#include <iostream>
#include <queue>
#include <vector>

using std::cout;
using std::endl;
using std::priority_queue;
using std::vector;

//course4
// 笔记by lcl:\
一) 优先级队列(堆排序) 类似插队
/*
std::priority_queue
 C++ Containers library std::priority_queue 
Defined in header <queue>
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
可以看到, 上述的value_type是容器(比如: vector)里面有的成员, 就看传进来的T是什么类型\
这就是类型萃取, 不用针对具体的容器写其类型, 有类名作用域限定符提取\

同时,当想传第三个参数时, 第二个参数也不能省掉, 也要显示地写出来
最容易错误的 .   第二个参数是让优先级队列底层用什么东西存数据

模板声明带有三个参数，priority_queue<Type, Container, Functional>
　　Type 为数据类型， Container 为保存数据的容器， Functional 为元素比较方式。
　　Container 必须是用数组实现的容器，比如 vector, deque 但不能用list.
　　
STL里面默认用的是vector. 比较方式默认用operator< , 所以如果你把后面俩个
　　参数 缺省的话，优先队列就是大顶堆，队头元素最大。


*/




/* operator<(const A &lhs, const A &rhs); */
void test()
{
    //优先级队列底层使用的是堆排序，默认情况是一个大顶堆（std::less）
    //当有新的元素插进来的时候，就会用新的元素与堆顶去进行比较，
    //堆顶作为左边的操作数，新的元素作为了右操作数，堆顶与新的元素
    //去进行比较，如果满足std::less，就不用新的元素与堆顶进行置换，
    //如果不满足条件，堆顶依旧是新的堆顶
    priority_queue<int> number;

    vector<int> vec = {1, 5, 9, 7, 3, 6, 2};
    // 把vector里面的数据一个个插入到优先级队列里面
    for(size_t idx = 0; idx != vec.size(); ++idx)
    {
        number.push(vec[idx]);//根据打印结果, 值越大优先级越高
        cout << "优先级最高的元素 : " << number.top() << endl;
    }
    // 默认是std::less, 然后打印的时候从大到小打印出来的, 这是为什么?\
    1 在 top, 4 大于 1, 置换为 4 1 ...
    //当

    while(!number.empty())
    {
        cout << number.top() << "   ";
        number.pop();
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
};

std::ostream &operator<<(std::ostream &os, const Point &rhs)
{
    os << "(" << rhs._ix 
      << "," << rhs._iy
      << ")";
    return os;
}

bool operator<(const Point &lhs, const Point &rhs)
{
    /* cout << "bool operator<(const Point &, const Point &)" << endl; */
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
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
}

bool operator>(const Point &lhs, const Point &rhs)
{
    /* cout << "bool operator>(const Point &, const Point &)" << endl; */
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
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
}

struct Comparetion
{
    bool operator()(const Point &lhs, const Point &rhs) const
    {
        /* cout << "struct Comparetion" << endl; */
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
            else
            {
                return false;
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

    vector<Point> vec = {
        Point(1, 2),
        Point(1, 2),
        Point(1, -2),
        Point(-1, 2),
        Point(11, 2),
        Point(1, 12),
    };
    /* priority_queue<Point> number; */
    /* priority_queue<Point, vector<Point>, std::greater<Point>> number; */
    priority_queue<Point, vector<Point>, Comparetion> number;

    for(size_t idx = 0; idx != vec.size(); ++idx)
    {
        number.push(vec[idx]);
        cout << "优先级最高的元素 : " << number.top() << endl;
    }

    while(!number.empty())
    {
        cout << number.top() << "   ";
        number.pop();
    }
    cout << endl;

}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

