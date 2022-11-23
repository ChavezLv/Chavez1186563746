#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::ostream_iterator;
using std::vector;
using std::copy;

//course1 
// 笔记by lcl:\

//#include <iterator>

// 为什么要定义这么多迭代器? 
/*
迭代器的类别?

流迭代器(输入流迭代器, 输出流迭代器) == 容器? 
 -- 是容器概念的扩充
迭代器看成是容器;
输出流(cout的缓冲区, 容器);输入流(缓冲区, 容器);

copy(算法)  头文件
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
  因为vector迭代器是随机访问迭代器, 功能是最大的. 具体可见: 
    file:///C:/Users/Charlie%20Lee/Desktop/%E6%89%8B%E5%86%8C%E7%B1%BB/cpp_reference/reference/en/cpp/iterator.html








*/


//cout 1 4 8 9 0 "\n"
void test()
{
    vector<int> number = {1, 4, 8, 9, 3};
    ostream_iterator<int> osi(cout, "\n"); // 首先调用构造函数
    copy(number.begin(), number.end(), osi);// 拷贝到osi里面了
    /*
    //分析这三句: 看源码stl_iterator.h
    
      template<typename _II, typename _OI>
    inline _OI
    copy(_II __first, _II __last, _OI __result)
    {
      // concept requirements
      __glibcxx_function_requires(_InputIteratorConcept<_II>)
      __glibcxx_function_requires(_OutputIteratorConcept<_OI,
        typename iterator_traits<_II>::value_type>)
      __glibcxx_requires_valid_range(__first, __last);

      return (std::__copy_move_a2<__is_move_iterator<_II>::__value>
          (std::__miter_base(__first), std::__miter_base(__last),
           __result));
    }


    总结: 这三句, 一开始建立一个对象osi, 传入参数, 底层用到了__copy()函数
    里面对 osi对象先解引用, 解引用返回对象本身, 则间接是赋值运算符函数
    */
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

