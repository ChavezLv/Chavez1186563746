#include <iostream>
#include <set>
#include <map>
#include <utility>
#include <string>

using std::cout;
using std::endl;
using std::set;
using std::map;
using std::pair;
using std::string;

void test()
{
    //set的特点：
    //1、关键字必须唯一，不能重复
    //2、默认情况下，set中的key会按照升序进行排序
    //3、set的底层实现是红黑树
    //int a = 10;
    //vector<int> number;
    set<int> number = {1, 3, 5, 8, 9, 3, 4, 3, 5};//1、使用大括号初始化

    /* int arr[10] = {1, 8, 9, 5, 3, 7, 2, 3}; */
    /* set<int> number(arr, arr + 10);//2、使用迭代器范围的形式 */

    //遍历set
    //迭代器可以看成是一个高级指针
    set<int>::iterator it;
    for(it = number.begin(); it != number.end(); ++it)
    {
        cout << *it << "  ";
    }
    cout << endl;

    /* auto a = 10; */
    for(auto &elem : number)
    {
        cout << elem << "  ";
    }
    cout << endl;

    //查找
    cout <<endl;
    size_t cnt1 = number.count(9);
    size_t cnt2 = number.count(10);
    cout << "cnt1 = " << cnt1 << endl;
    cout << "cnt2 = " << cnt2 << endl;

    cout << endl;
    /* auto it2 = number.find(10); */
    auto it2 = number.find(9);
    if(it2 == number.end())
    {
        cout << "该元素不存在set中" << endl;
    }
    else
    {
        cout << *it2 << endl;
    }
    cout << endl;

    //插入
    cout << endl;
    pair<set<int>::iterator, bool> ret = number.insert(7);
    if(ret.second)
    {
        cout << "插入成功" << endl;
    }
    else
    {
        cout << "插入失败" << endl;
    }
    for(auto &elem : number)
    {
        cout << elem << "  ";
    }
    cout << endl;

    //下标访问,set不支持下标访问
    cout << endl;
    /* cout << number[0] << endl;//error */

    //不能进行修改，为了保证红黑树结构的稳定
    cout <<endl << "修改" << endl;
    it = number.begin();
    cout << *it << endl;
    /* *it = 100;//error */
}

void test00()
{
    pair<int, string> number{1, "helloworld"};
    cout << number.first << "  " << number.second << endl;
}

void test2()
{
    //map特点
    //1、key值必须唯一，不存在相同的key，但是value值可以相同，也可以不同
    //2、默认情况下，按照key值进行升序排列
    //3、set的底层实现是红黑树
    map<string, string> number = {
        pair<string, string>("123", "wuhan"),
        pair<string, string>("hello", "shanghai"),
        pair<string, string>("123", "wuhan"),//两个正好是pair, 用pair初始化
        {"345", "nanjing"},//两个值正好也是pair, 用两个值来存, 
        {"0755", "shenzhen"},
        {"8888", "shenzhen"}
    };

    //遍历
    for(auto &elem : number)
    {
        cout << elem.first << " " << elem.second << endl;
    }
    
    //查找
    cout <<endl;
    size_t cnt1 = number.count("123");
    size_t cnt2 = number.count("wuhan");
    cout << "cnt1 = " << cnt1 << endl;
    cout << "cnt2 = " << cnt2 << endl;

    cout << endl;
    /* auto it2 = number.find("wuhan"); */
    auto it2 = number.find("123");
    if(it2 == number.end())
    {
        cout << "该元素不存在map中" << endl;
    }
    else
    {
        cout << it2->first << " " << it2->second << endl;
    }
    cout << endl;

    //插入
    /* pair<map<string, string>::iterator, bool> ret = */
    /*     number.insert(pair<string, string>("190", "xinjiang")); */
    auto ret = number.insert({"190", "xinjiang"});
    if(ret.second)
    {
        cout << "插入成功" << ret.first->first 
            << "  " << ret.first->second << endl;
    }
    else
    {
        cout << "插入失败" << endl;
    }
    for(auto &elem : number)
    {
        cout << elem.first << " " << elem.second << endl;
    }

    //map下标访问
    //如果key值存在就会返回key值对应的value，如果key不存在，就返回空
    //并且插入到map之中
    cout << endl;
    cout << "number[\"123\"] = " << number["123"] << endl;
    cout << "number[\"9999\"] = " << number["9999"] << endl;
    for(auto &elem : number)
    {
        cout << elem.first << " " << elem.second << endl;
    }

    cout << endl << endl;
    number["9999"] = "beijing";//map的[]具有修改功能
    for(auto &elem : number)
    {
        cout << elem.first << " " << elem.second << endl;
    }

    cout << endl;
    /* number["123"] = "beijing"; */
    number.operator[]("123").operator=("beijing");
    for(auto &elem : number)
    {
        cout << elem.first << " " << elem.second << endl;
    }
    
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}

