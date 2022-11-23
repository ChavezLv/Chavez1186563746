#include <math.h>
#include <iostream>
#include <unordered_map>
#include <string>

using std::cout;
using std::endl;
using std::unordered_multimap;
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
    unordered_multimap<int, string> number = {
        {1, "wuhan"},
        {2, "shanghai"},
        std::pair<int, string>(4, "nanjing"),
        std::make_pair(2, "hehe"),
        std::make_pair(5, "新疆"),
        std::pair<int, string>(4, "nanjing"),
        {2, "shanghai"},
        std::pair<int, string>(3, "nanjing"),
    };
    display(number);

    cout << endl << "unordered_multimap的插入insert" << endl;
    /* auto ret = number.insert(std::pair<int, string>(6, "hainan")); */
    /* auto ret = number.insert({6, "hainan"}); */
    /* auto ret = number.insert(std::make_pair(6, "hainan")); */
    auto ret = number.insert(unordered_multimap<int, string>::value_type(6, "hainan"));//类型萃取

    //unordered_multimap是不支持下标访问的，因为key值可以相同，所以可能有二义性
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
    unordered_multimap<string, Point> number = {
        {"wuhan", Point(1, 2)},
        {"beijing", Point(1, 2)},
        std::pair<string, Point>("nanjing", Point(3, 4)),
        std::make_pair("nanjing", Point(10, 20))
    };
    display(number);
#if 0
    //unordered_multimap不支持下标访问
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

