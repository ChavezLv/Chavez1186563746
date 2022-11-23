#include <math.h>
#include <iostream>
#include <unordered_set>

using std::cout;
using std::endl;
using std::unordered_set;

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
一) 
template<
    class Key,
    class Hash = std::hash<Key>,  //hash里面也是对函数调用运算符做了一个重载()\
    可以借鉴这个来设计hash函数
    class KeyEqual = std::equal_to<Key>,//也重载了 (), 
    class Allocator = std::allocator<Key>
> class unordered_set;

//二)特点: key值唯一, 元素无序\
unordered_set<int> number = {10, 4, 6, 7, 8, 7, 3, 4, 7, 2};

//三) insert和erase无区别\
count 和 find 也无区别\

//四) 自定义类型该如何写? 
Standard specializations for basic types

Defined in header <functional>

template<> struct hash<bool>;
template<> struct hash<char>;
template<> struct hash<signed char>;
template<> struct hash<unsigned char>;
template<> struct hash<char16_t>;
template<> struct hash<char32_t>;
template<> struct hash<wchar_t>;
template<> struct hash<short>;
template<> struct hash<unsigned short>;
template<> struct hash<int>;
template<> struct hash<unsigned int>;
template<> struct hash<long>;
template<> struct hash<long long>;
template<> struct hash<unsigned long>;
template<> struct hash<unsigned long long>;
template<> struct hash<float>;
template<> struct hash<double>;
template<> struct hash<long double>;
template< class T > struct hash<T*>;

//内置类型都写了的, 但是自定义类型没有写\
std::hash<Key>::operator()  //这里有个例子, 

//hash函数进行特化(特殊版本的hash)
namespace  std  //扩展标准命名空间
{

template <>
struct hash<Point> //可以用class也可以struct(不用写public)
{
    size_t operator()(const Point &rhs) const;
};//end of struct hash<Point>

}//end of namespace std

//这个和上面的也是等效的,但是上述方式值得借鉴\
unordered_set<Point, HasherPoint> 要写明
// 对于前面std::less也可以写成扩展标准命名空间的
struct HasherPoint
{//...
}

 // insert无区别

//接下来看下unorderd_multiset




//------------------------以上都是笔记--------------------


void test()
{
    unordered_set<int> number = {10, 4, 6, 7, 8, 7, 3, 4, 7, 2};
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

//hash函数进行特化(特殊版本的hash)
namespace  std  //扩展标准命名空间
{

template <>
struct hash<Point> //可以用class也可以struct(不用写public)
{
    size_t operator()(const Point &rhs) const
    {
        return (rhs.getX() << 1 ) ^ (rhs.getY() << 2);
    }

};//end of struct hash<Point>

}//end of namespace std

//这个和上面的也是等效的,但是上述方式值得借鉴\
unordered_set<Point, HasherPoint> 要写明
// 对于前面std::less也可以写成扩展标准命名空间的
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
struct equal_to<Point> // 重载 = 号, 因为equal_to里面有一个比较
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
    /* unordered_set<Point> number = { */
    unordered_set<Point, HasherPoint> number = {
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

