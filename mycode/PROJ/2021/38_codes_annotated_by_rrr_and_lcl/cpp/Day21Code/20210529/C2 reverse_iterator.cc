#include <iostream>
#include <iterator>
#include <vector>

using std::cout;
using std::endl;
using std::vector;


int main(int argc, char **argv)
{
    vector<int> number = {1, 3, 5, 7, 9};
    vector<int>::reverse_iterator rit = number.rbegin();
    // 任何一个容器都可以用?  reverse_iterator 反向迭代器
    //表面++, 底层是--
    for(; rit != number.rend(); ++rit)
    {
        cout << *rit << "   ";
    }
    cout << endl;
    return 0;
}

//stl源码剖器 第二章 第一级空间配置器, 第二级空间配置器预习 !!! 重要
//自己实现vector

//还有算法

//remove和 remove_if
// for_each()
// bind


