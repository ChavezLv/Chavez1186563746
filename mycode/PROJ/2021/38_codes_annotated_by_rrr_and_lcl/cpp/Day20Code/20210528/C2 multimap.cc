#include <math.h>
#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::multimap;
using std::string;

template <typename Container>
void display(const Container &c)
{
    for(auto &elem : c)
    {
        cout << elem.first << "  "
             << elem.second << endl;
    }
    cout << endl;
}


void test()
{
    //multimap的特点:

    //key值是不唯一的, 可以重复, value值也是可以重复的\
    默认情况下, 会按照key进行升序排列\

    multimap<int, string> number = {
        {1, "wuhan"},
        {2, "shanghai"},
        {2, "shanghai"},
        std::pair<int, string>(3, "nanjing"),
        std::pair<int, string>(4, "nanjing"),
        std::make_pair(2, "hehe"),
        std::make_pair(5, "新疆"),
        std::pair<int, string>(4, "nanjing"),
    };
    display(number);

    cout << endl << "multimap的插入insert" << endl;
    /* auto ret = number.insert(std::pair<int, string>(6, "hainan")); */
    /* auto ret = number.insert({6, "hainan"}); */
    /* auto ret = number.insert(std::make_pair(6, "hainan")); */
    auto ret = number.insert(multimap<int, string>::value_type(6, "hainan"));//类型萃取

    //multimap的查找, 和之前不一样的就是, size_t cnt1 = number.count("2");\
    返回值不只有 0 和 1

    //multimap是不支持下标访问的，因为key值可以相同，所以可能有二义性
#if 0
    cout << endl << endl;
    cout << "number[10] = " << number[10] << endl;
    cout << "number[1] = " << number[1] << endl;
    display(number);

    number[10] = "helloworld";
    display(number);
#endif
}

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
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

void test2()
{
    multimap<string, Point> number = {
        {"wuhan", Point(1, 2)},
        {"beijing", Point(1, 2)},
        std::pair<string, Point>("nanjing", Point(3, 4)),
        std::make_pair("nanjing", Point(10, 20))
    };
    display(number);
#if 0
    //multimap不支持下标访问\
    为什么? \
    因为key值是有可能相同的, 就是下标访问运算符里面的值是可以相同的\
    则同一个key就会多个value值, 就会产生二义性
    cout << endl << endl;
    cout << "number[\"nanjing\"] = " << number["nanjing"] << endl;
    cout << "number[\"jing\"] = " << number["jing"] << endl;

    cout << endl;
    display(number);
    number["jing"] = Point(10, 20);
    display(number);
#endif
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

