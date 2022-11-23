#include <iostream>
#include <vector>
#include <deque>
#include <list>

using std::cout;
using std::endl;
using std::vector;
using std::deque;
using std::list;
//course1
// 笔记by lcl:\
一.test()\
vector/deque/list   初始化方式 都一样\
1) 空的\
2) count的\
3) 迭代器范围的, 迭代器 ~ 广义的指针\
vector<int>::const_iterator cit;//广义指针\
\
4) 拷贝构造的\
5) 移动构造的\
遍历方式:\
四种!

//\
二.test1()--deque\
直接复制void test() {} 然后改一下:%s/.../.../g即可

//\
二.test2()---list\
直接复制void test() {} 然后改一下:%s/.../.../g, 此时第一种遍历方式有问题\
因为没有重载运算符[]

//\
三. vector的尾部的插入与删除\
看sequenceContainer2.cc\
deque和list也是一样的\
头插法和尾插法


//\
四. 为什么vector没有头插法, 头部删除法? 看9.1标准模板库.ppt的图示\
sequenceContainer2.cc\
因为, vector如果有头插, 一个元素, 剩余元素要一直往后移动, 删除也是, 时间复杂度比较高

//\
五. 分析vector源码:\
1)vector内部三个指针? 开头, capacity, size\
2)类型萃取技术\
cout << "sizeof(vector<int>) = " << sizeof(vector<int>) << endl;\
cout << "sizeof(vector<double>) = " << sizeof(vector<double>) << endl << endl;\
cout << "sizeof(vector<char>) = " << sizeof(vector<char>) << endl << endl;\
step:  在slickedit 搜索 Filename :  vector\
鼠标放在 #include <bits/stl_vector.h> 这里  右键 鼠标, go to include file: <***.h>\
要Esc先退出编辑界面, 然后类似vim一样直接搜索 vector 或者直接看defs的预览
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>{

    }
//再右键看下基类的定义   右键一下大括号, 会变蓝, 方便找到{}的范围
      template<typename _Tp, typename _Alloc>
    struct _Vector_base
    {
         public:
            //一下这些 在cppreference的member types都有体现, 比如, value_type等等
            //其他类型 deque  set 等等也都有类似的typedef
            //问:  为什么要这么写? 
            //   为了更通用, 使用时, 通过类名作用域+限定符 获取
            // 一个类的类型, 不是一个类的指针, 类似于化学的萃取, 只要针对每一种类, 把类型拿出来\
      靠typedef 就能提取
      typedef _Tp                   value_type;
      typedef typename _Base::pointer           pointer;
      typedef typename _Alloc_traits::const_pointer const_pointer;
      typedef typename _Alloc_traits::reference     reference;
      typedef typename _Alloc_traits::const_reference   const_reference;
      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      //旧版里面是:\
      typedef _Tp  value_type;\
      typedef value_type * iterator; \
               可以看到迭代器就是一个_Tp类型的指针
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
      const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator>       reverse_iterator;
      typedef size_t                    size_type;
      typedef ptrdiff_t                 difference_type;
      typedef _Alloc                    allocator_type;
    }
    //看到三个指针了? 旧版本是继承和这里不同 三个指针成员一直吸收过来 \
    view -> line numbers 可以显示行号, ppt有旧版图示
       template<typename _Tp, typename _Alloc>
    struct _Vector_base
    {
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
    rebind<_Tp>::other _Tp_alloc_type;
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
        pointer;

      struct _Vector_impl
      : public _Tp_alloc_type
      { 
        //... 初始化一个vector之后, 这三个指针是在栈上面的, \
        直接解引用是取不到地址的  \
        &vecNums;//error 只能拿到指针, 不能打印第一个元素的地址 \
        &vecNums[0]; //ok\
        &*vecNums.begin(); // ok\
        int *pdata = number.date();//ok
        //...
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;
    }


//对比deque:
    template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class deque : protected _Deque_base<_Tp, _Alloc>
    {
         public:
      typedef _Tp                   value_type;
      typedef typename _Alloc_traits::pointer       pointer;
      typedef typename _Alloc_traits::const_pointer const_pointer;
      typedef typename _Alloc_traits::reference     reference;
      typedef typename _Alloc_traits::const_reference   const_reference;
      typedef typename _Base::iterator          iterator;
      //另外一个类的类型, 不是一个类的指针, 类似于化学的萃取, 只要针对每一种类, 把类型拿出来\
      靠typedef 就能提取
      typedef typename _Base::const_iterator        const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator>       reverse_iterator;
      typedef size_t                    size_type;
      typedef ptrdiff_t                 difference_type;
      typedef _Alloc                    allocator_type;

    }
//3) 然后iterator看完之后, 就看到 end begin 就是返回return 一个it\
cppreference官网 的std::set  rbegin   \
   可以看到begin end 并不是绝对位置, 而是相对位置, 比如链表的逆置, 在stl有\
   相应的函数可以用的, 就像内存空间一样, 用的不是实际的空间, 否则一个代码就是4g了!

//4)然后看其他函数  \
size()    \
max_size() {  size_type(-1)  / sizeof(_Tp);//为什么用的是-1?\
就是计组里面用的时原码补码, -1 是原码最大值, sizeof数据大小, 比如是4 }\
\
empty()   {  return begin() == end(); }\
\
operator[](_n) { return  *(begin() + _n);  }

// 5) [面试题有问过]然后看一下at 和 []的区别? \
看源码
      /// Safety check used only from at().
      void
      _M_range_check(size_type __n) const
      {
    if (__n >= this->size())
      __throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
                       "(which is %zu) >= this->size() "
                       "(which is %zu)"),
                   __n, this->size());
      }

      at(size_type __n)
      {
    _M_range_check(__n);
    return (*this)[__n];
      }
//这是新版的源码, 和老师的不一样.  可以看到更安全

// 6) push_back 如何扩容的? 
void
      push_back(const value_type& __x)
      {
    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)//有空闲位置
      {
        _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                     __x);
        ++this->_M_impl._M_finish;//插入后++
      }
    else//没有空闲位置  
      _M_realloc_insert(end(), __x);//再右键看下它的实现
      }
// _M_realloc_insert(end(), __x)的内部部分内容
            size_type
      _M_check_len(size_type __n, const char* __s) const
      {
    if (max_size() - size() < __n)
      __throw_length_error(__N(__s));

    const size_type __len = size() + std::max(size(), __n);
    return (__len < size() || __len > max_size()) ? max_size() : __len : 1;
      }
//旧版是这样的:  const size_type __len = __old_size != 0 ? 2 * __old_size : 1;\
旧空间 != 0 申请一个就可以, 否则就是两倍扩容.


//\ 扩容之后就是重新置位!
      template<typename _Integer>
    void
    _M_initialize_dispatch(_Integer __n, _Integer __value, __true_type)
    {
      this->_M_impl._M_start = _M_allocate(static_cast<size_type>(__n));
      this->_M_impl._M_end_of_storage =
        this->_M_impl._M_start + static_cast<size_type>(__n);
      _M_fill_initialize(static_cast<size_type>(__n), __value);
    }

//---------------------------------以上都是源码阅读-----------------------------------//
void test()
{
    //初始化
    /* vector<int> number;//1、创建空对象 */
    /* vector<int> number(10, 1);//2、初始化count个value, 初始化10个1 */
    /* int arr[10] = {1, 5, 7, 9, 4, 6, 8, 2, 1, 20}; */
    /* vector<int> number(arr, arr + 10);//3、迭代器范围,[arr, arr + 10) */
    vector<int> number = {1, 5, 7, 9, 4, 6, 2, 1, 20};//4、大括号

    //遍历方式
    //1)
    for(size_t idx = 0; idx != number.size(); ++idx)
    {
        cout << number[idx] << "  ";
    }
    cout << endl;
    //2) it for
    vector<int>::iterator it;
    for(it = number.begin(); it != number.end(); ++it)
    {
        cout <<*it << "  ";
    }
    cout << endl;
    //3) auto  it for
    auto it2 = number.begin();
    for(; it2 != number.end(); ++it2)
    {
        cout <<*it2 << "  ";
    }
    cout << endl;
    //4) for  : 
    for(auto &elem :number)//auto推导变量, "&"减少拷贝操作
    {
        cout << elem << "  ";
    }
    cout << endl;

}

void test2()
{
    //初始化
    /* deque<int> number;//1、创建空对象 */
    /* deque<int> number(10, 10);//2、初始化count个value */
    /* int arr[10] = {1, 5, 7, 9, 4, 6, 8, 2, 1, 20}; */
    /* deque<int> number(arr, arr + 10);//3、迭代器范围,[arr, arr + 10) */
    deque<int> number = {1, 5, 7, 9, 4, 6, 2, 1, 20};//4、大括号

    //遍历
    for(size_t idx = 0; idx != number.size(); ++idx)
    {
        cout << number[idx] << "  ";
    }
    cout << endl;

    deque<int>::iterator it;
    for(it = number.begin(); it != number.end(); ++it)
    {
        cout <<*it << "  ";
    }
    cout << endl;

    auto it2 = number.begin();
    for(; it2 != number.end(); ++it2)
    {
        cout <<*it2 << "  ";
    }
    cout << endl;

    for(auto &elem :number)
    {
        cout << elem << "  ";
    }
    cout << endl;

}

void test3()
{
    //初始化
    /* list<int> number;//1、创建空对象 */
    /* list<int> number(10, 10);//2、初始化count个value */
    /* int arr[10] = {1, 5, 7, 9, 4, 6, 8, 2, 1, 20}; */
    /* list<int> number(arr, arr + 10);//3、迭代器范围,[arr, arr + 10) */
    list<int> number = {1, 5, 7, 9, 4, 6, 2, 1, 20};//4、大括号

    //遍历
    //list不支持下标访问
#if 0
    for(size_t idx = 0; idx != number.size(); ++idx)
    {
        cout << number[idx] << "  ";
    }
    cout << endl;
#endif

    list<int>::iterator it;
    for(it = number.begin(); it != number.end(); ++it)
    {
        cout <<*it << "  ";
    }
    cout << endl;

    auto it2 = number.begin();
    for(; it2 != number.end(); ++it2)
    {
        cout <<*it2 << "  ";
    }
    cout << endl;

    for(auto &elem :number)
    {
        cout << elem << "  ";
    }
    cout << endl;

}
int main(int argc, char **argv)
{
    test3();
    return 0;
}

