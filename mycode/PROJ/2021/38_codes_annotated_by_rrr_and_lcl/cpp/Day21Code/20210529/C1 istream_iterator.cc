#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::ostream_iterator;
using std::istream_iterator;
using std::vector;
using std::copy;

void test()
{
    vector<int> number;
    cout << "111" << endl;
    istream_iterator<int> isi(std::cin);// 这里需要输入cin一次, 接收一个值到vector
    // cin 构造 isi ---> read() --> cin --> 
    cout << "222" << endl;

    //push_back()返回的是void
    //对于vector插入要push_back(), 但是begin()只能满足参数要求, 不具有类似的特点
    // 使用函数back_inserter替代, 传一个容器进入, 返回迭代器的适配器
    /* copy(isi, istream_iterator<int>(), number.begin());//error,vector的插入要使用push_back */
    copy(isi, istream_iterator<int>(), std::back_inserter(number));// 这里可以输入cin多次, 接收多个值到vector, ctrl + d退出
    //std::back_inserter和std::back_inserter_iterator也是可以互换使用的.
    cout << "333" << endl;

    copy(number.begin(), number.end(), ostream_iterator<int>(cout, "\n"));
    cout << "444" << endl;

    //总结: copy这种也属于数组遍历的方法

}

int main(int argc, char **argv)
{
    test();
    return 0;
}

