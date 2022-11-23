#include <math.h>
#include <iostream>
#include <unordered_set>

using std::cout;
using std::endl;
using std::unordered_multiset;

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
一)  特点: key 可以重复  key没有顺序



void test()
{
    unordered_multiset<int> number = {10, 4, 6, 7, 8, 7, 3, 4, 7, 2};
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

//hash函数进行特化
namespace  std
{

template <>
struct hash<Point>
{
    size_t operator()(const Point &rhs) const
    {
        return (rhs.getX() << 1 ) ^ (rhs.getY() << 2);
    }

};//end of struct hash<Point>

}//end of namespace std

struct HasherPoint
{
    size_t operator()(const Point &rhs) const
    {
        cout << "struct HasherPoint" << endl;
        return (rhs.getX() << 1 ) ^ (rhs.getY() << 2);
    }

};

namespace  std
{
template <>
struct equal_to<Point>
{
    bool operator()(const Point &lhs, const Point &rhs)  const
    {
        cout << "struct equal_to" << endl;
        return (lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY());
    }

};//end of struct hash<Point>

}//end of namespace std

#if 0
bool operator==(const Point &lhs, const Point &rhs) 
{
    return (lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY());
}
#endif
void test2()
{
    /* unordered_multiset<Point> number = { */
    unordered_multiset<Point, HasherPoint> number = {
        Point(1, 2),
        Point(1, -2),
        Point(-1, 2),
        Point(1, 2),
        Point(10, 2),
        Point(-12, 2),
        Point(1, 12),
    };
    display(number);
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

