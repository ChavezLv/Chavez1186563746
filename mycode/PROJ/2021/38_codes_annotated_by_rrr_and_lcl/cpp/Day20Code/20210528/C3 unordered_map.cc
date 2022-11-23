#include <math.h>
#include <iostream>
#include <unordered_map>
#include <string>

using std::cout;
using std::endl;
using std::unordered_map;
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
//course3 
// 笔记by lcl:\
一) 特点 : key 值唯一, 不重复;  无序 \
二) map最重要的就是它的下标, 同样具有查找和插入的功能 \
三) 然后看自定义类型   构造时编译string不需要重载->演示下标\
同样具有查找 插入和修改的功能

演示unordered_multimap.cc



void test()
{
    unordered_map<int, string> number = {
        {1, "wuhan"},
        std::pair<int, string>(3, "nanjing"),
        std::pair<int, string>(4, "nanjing"),
        std::make_pair(2, "hehe"),
        std::make_pair(5, "新疆"),
        {2, "shanghai"},
    };
    display(number);

    cout << endl << "unordered_map的插入insert" << endl;
    /* auto ret = number.insert(std::pair<int, string>(6, "hainan")); */
    /* auto ret = number.insert({6, "hainan"}); */
    /* auto ret = number.insert(std::make_pair(6, "hainan")); */
    auto ret = number.insert(unordered_map<int, string>::value_type(6, "hainan"));//类型萃取
    if(ret.second)
    {
        cout << "插入成功 " << ret.first->first << "  "
             << ret.first->second << endl;
    }
    else
    {
        cout << "插入失败" << endl;
    }

    cout << endl << endl;
    cout << "number[10] = " << number[10] << endl;
    cout << "number[1] = " << number[1] << endl;
    display(number);

    number[10] = "helloworld";
    display(number);
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
    unordered_map<string, Point> number = {
        {"wuhan", Point(1, 2)},
        {"beijing", Point(1, 2)},
        std::pair<string, Point>("nanjing", Point(3, 4)),
        std::make_pair("nanjing", Point(10, 20))
    };//同样是无序和不可重复, 同样string有重载, 不会报错\
    Helper classes \
    std::hash<std::string> \
    string的也写好了的, 因此不会报错.


    display(number);

    cout << endl << endl;
    cout << "number[\"nanjing\"] = " << number["nanjing"] << endl;
    cout << "number[\"jing\"] = " << number["jing"] << endl;

    cout << endl;
    display(number);
    number["jing"] = Point(10, 20);
    display(number);
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

