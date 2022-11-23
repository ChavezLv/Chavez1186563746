#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::mem_fn;
//course1 
// 笔记by lcl:
/*
主要针对成员函数, 的成员函数适配器
----------------------------mem_fn------------------------------------

for_each

    std::for_each(numbers.begin(), numbers.end(), mem_fn(&Number::print));

前面讲的所有的算法都是这样, 稍微注意注意一下就可以了, 如果算法里面要用到就用一下就可以了


重点: 容器自定义的函数对象, 流迭代器, 函数适配器 等等


最后stl里面还有一个大boss-------->>>
-------------------------allocator---------------------------
重点四个函数: 
-----------construct  destroy  allocate  deallocate----------

------------------37期代码, 的下一天就是course4--------------
--------------看day23的----------



*/


class Number
{
public:
    Number(size_t data = 0)
    : _data(data)
    {
    }

    void print() const
    {
        cout << _data << " ";

    }

    bool isEven() const
    {
        return (0 == _data % 2);

    }

    bool isPrime() const
    {
        if(1 == _data)
        {
            return false;

        }
        //质数/素数
        for(size_t idx = 2; idx <= _data/2; ++idx)
        {
            if(0 == _data % idx)
            {
                return false;
            }
        }
        return true;
    }
private:
    size_t _data;
};

void test()
{
    vector<Number> numbers;
    for(size_t idx = 1; idx != 10; ++idx)
    {
        numbers.push_back(Number(idx));
    }

    std::for_each(numbers.begin(), numbers.end(), mem_fn(&Number::print));
    cout << endl;

    //erase函数参数之前说过，两个参数，
    numbers.erase(remove_if(numbers.begin(), numbers.end(), mem_fn(&Number::isEven)), numbers.end());
    std::for_each(numbers.begin(), numbers.end(), mem_fn(&Number::print));
    cout << endl;

    numbers.erase(remove_if(numbers.begin(), numbers.end(), mem_fn(&Number::isPrime)), numbers.end());
    std::for_each(numbers.begin(), numbers.end(), mem_fn(&Number::print));
    cout << endl;
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
