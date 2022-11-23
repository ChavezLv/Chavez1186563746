#include <math.h>
#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::map;
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

//course2 
// 笔记by lcl:\
 set和map的区别 : 把构造时的class Key, class T看成是一个整体\
 一个整体的pair, 这样和set就是一样的了\
 //一) 初始话的make_pair()  \
 为什么可以看成是pair, 因为map进行初始化的时候, 就是一个pair类型\
\
 make_pair不是类模板, 是函数模板, 所以也可以用这个来初始化,正好也是传两个值\
 //二) insert
void test()
{
    map<int, string> number = {
        {1, "wuhan"},
        {2, "shanghai"},
        std::pair<int, string>(3, "nanjing"),
        std::pair<int, string>(4, "nanjing"),
        std::make_pair(2, "hehe"),
        std::make_pair(5, "新疆")
    };
    display(number);


    //插入insert\
    返回的结果正好是, pair类型\
    而multimap的插入, 不能像map这样if(ret.second)判断, 因为必定成功
    cout << endl << "map的插入insert" << endl;
    /* auto ret = number.insert(std::pair<int, string>(6, "hainan")); */
    /* auto ret = number.insert({6, "hainan"}); // 也可以用大括号的形式*/
    /* auto ret = number.insert(std::make_pair(6, "hainan")); make_pair返回正好是insert对应的pair*/
    auto ret = number.insert(map<int, string>::value_type(6, "hainan"));//类型萃取
    if(ret.second)
    {
        cout << "插入成功 " << ret.first->first << "  "
             << ret.first->second << endl;
    }
    else
    {
        cout << "插入失败" << endl;
    }

    //删除
    cout << endl << "map delete" << endl;
    auto it3 = number.begin();
    cout << it3->first << " " << it3->second << endl;
    number.erase(it3);
    cout << endl << endl;
    display(number);

    //map的下标访问, 
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
        cout<< "Point(int = 0, int = 0)" <<endl;
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
    //1)为什么没有重载Point的运算符也没有报错? \
    因为map的第三个模板参数默认用的less是对key值进行比较的\
    而key值的类型是一个string , basic_string是有实现比较运算符的重载的\
    所以不会报错
    map<string, Point> number = {
        {"wuhan", Point(1, 2)},
        {"beijing", Point(1, 2)},
        std::pair<string, Point>("nanjing", Point(3, 4)),
        std::make_pair("nanjing", Point(10, 20))
    };
    display(number);

    cout << endl << endl;
    cout << "number[\"nanjing\"] = " << number["nanjing"] << endl;
    cout << "number[\"jing\"] = " << number["jing"] << endl;

    cout << endl;
    display(number);
    number["jing"] = Point(10, 20);
    display(number);

    //2) 查找find
    //查找(1)
    cout << endl << "map count" <<endl;
    size_t cnt1 = number.count("3");

    //查找(2)
    cout << endl << "map find" << endl;
    auto it = number.find("3");
    if(it == number.end()){
        //...find null
    }else{
        //it->first;
        //it->second;
    }


    //下标访问, 此时Point只有有参构造
    cout << endl << "map []" <<endl;
    cout << "number[\"1\"] = " << number["1"] << endl;//如果存在\
    则打印正常地value值, 如果不存在, 则用空替代, 所以会调用Point的构造函数\
    把无参对象打印出来
    display(number);
    //print:
    // number["1"] = Point(int = 0, int = 0)    //有参构造
    // (0 , 0)

    //比如, number["10"] = Point(1100, 2000); //临时对象, 赋值运算符

    // 然后看 multimap.cc  insert必定成功, 没有判断; 没有下标访问运算符
            //想想为什么?  
}

//后面如果选择, 怎么选择? \
根据时间复杂度 和迭代器的类型两个方面来选择\
\
容器的嵌套使用是很常见的, map<string, set<int>> \
map<string , shared_ptr<int>> // 这里不一定都是指向堆空间的\
vector<vector<int>> \



int main(int argc, char **argv)
{
    test2();
    return 0;
}

