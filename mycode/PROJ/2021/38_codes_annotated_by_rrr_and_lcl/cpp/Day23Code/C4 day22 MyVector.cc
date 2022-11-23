#include <iostream>
#include <memory>

using std::cout;
using std::endl;

//course1 
// 笔记by lcl:
/*

1) 注意destroy和deallocate的区别: 

        while(_finish != _start)
        {
            _alloc.destroy(--_finish);//对象的销毁, 销毁老空间上面的对象
        }
2) 注意copy

__first = isi
__last =istream_iterator<int>()
__result = back_inserter(number)

template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)
    *__result = *__first;//*__first没问题  *__result没问题,但是不可以赋值有问题, 
  return __result;
}

//新空间申请了,但是没有对象的, 不能赋值
T *pTemp = _alloc.allocate(newCapacity);
   
        a)错误写法
        copy(_start, _finish, pTemp); // 拷贝到一个流迭代器pTemp这里,    pTmp必须有对象

        因为源码里面: 
    *__result = *__first;//*__first没问题  *__result没问题,但是不可以赋值,有问题, 不合理!

--------b)正确写法: 使用未初始化的copy---------------------------
        std::uninitialized_copy(_start, _finish, pTemp);//老的空间上的元素也被拷贝了


3) 析构函数

if(_start)//没有对象时候
    {
        while(_finish != _start)
        {
            _alloc.destroy(--_finish);
        }
        _alloc.deallocate(_start, capacity());
    }


4) 使用for(auto &elem : number){} 遍历时需要写begin()和end()两个接口

5) c语言中重定义和c++11的区别
    a)语言
    typedef T * iterator;
    using iterator = T *;//C++11 


----------------------------slickedit-----------------------------
查看源码 include/memory--->> 头文件----->>
//空间的申请

_Tp* allocate(size_type __n, const void* = 0) 
{
    return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp))) : 0;
    //__n是否等于0, 如果等于0, 如果不等于0 
    //旧版源码: _Alloc, 往回找
    //typedef alloc _Alloc;
    //typedef malloc_alloc alloc;
    //typedef _malloc_alloc_template<0> malloc_alloc;
    //_malloc_alloc_template; // 类
  }

  // __p is not permitted to be a null pointer.
  void deallocate(pointer __p, size_type __n)
    { _Alloc::deallocate(__p, __n * sizeof(_Tp)); }



//对象的构建
void construct(pointer __p, const _Tp& __val) 
{ 
    new(__p) _Tp(__val); //定位new表达式，指定的空间上构建对象
    //就是传进来的是__p, 就不去new了
}

//对象的销毁
void destroy(pointer __p) { __p->~_Tp(); }

----------------源码这里有点乱,.新版不一样qaq------------------
*/

template<typename T>
class Vector
{
public:
    typedef T * iterator;
    /* using iterator = T *;//C++11 */
    
public:
    Vector();
    ~Vector();

    void push_back(const T &value); 
    void pop_back();    
    int size() const;
    int capacity() const;

    iterator begin()
    {
        return _start;
    }

    iterator end()
    {
        return _finish;
    }
private:
    void reallocate();//重新分配内存,动态扩容要用的
private:    
    static std::allocator<T> _alloc;

    T *_start;  //指向数组中的第一个元素
    T *_finish; //指向数组最后一个元素的下一个位置
    T *_end_of_storage; //指向数组最后位置的下一个位置
};

template <typename T>
std::allocator<T> Vector<T>::_alloc;

template <typename T>
Vector<T>::Vector()
: _start(nullptr)
, _finish(nullptr)
, _end_of_storage(nullptr)
{

}

template <typename T>
Vector<T>::~Vector()
{
    if(_start)
    {
        while(_finish != _start)
        {
            _alloc.destroy(--_finish);
        }
        _alloc.deallocate(_start, capacity());
    }

}

template <typename T>
void Vector<T>::push_back(const T &value)
{
    if(size() == capacity())
    {
        reallocate();
    }
    if(size() < capacity())
    {
        _alloc.construct(_finish++, value);
    }
}

template <typename T>
void Vector<T>::pop_back()
{
    if(size() > 0)
    {
        _alloc.destroy(--_finish);
    }
}
template <typename T>
int Vector<T>::size() const
{
    return _finish - _start;
}
template <typename T>
int Vector<T>::capacity() const
{
    return _end_of_storage - _start;
}

template <typename T>
void Vector<T>::reallocate()
{
    int oldCapacity = capacity();
    int newCapacity = 2 * oldCapacity > 0 ? 2 * oldCapacity : 1;

    T *pTemp = _alloc.allocate(newCapacity);
    if(_start)
    {
        /* copy(_start, _finish, pTemp); */
        std::uninitialized_copy(_start, _finish, pTemp);//老的空间上的元素也被拷贝了
        while(_finish != _start)
        {
            _alloc.destroy(--_finish);//对象销毁
        }
        _alloc.deallocate(_start, oldCapacity);//销毁老的空间
    }

    //指针指向新的空间
    _start = pTemp;
    _finish = pTemp + oldCapacity;
    _end_of_storage = pTemp +  newCapacity;
}

void printCapacity(const Vector<int> &vec)
{
    cout << "vec.size() = "<< vec.size() << endl
         << "vec.capacity() = " << vec.capacity() << endl;
}
void test()
{
    Vector<int> number;
    printCapacity(number);

    cout << endl;
    number.push_back(1);
    printCapacity(number);

    cout << endl;
    number.push_back(2);
    printCapacity(number);

    cout << endl;
    number.push_back(3);
    printCapacity(number);

    cout << endl;
    number.push_back(3);
    printCapacity(number);

    cout << endl;
    number.push_back(3);
    printCapacity(number);

    for(auto &elem : number)
    {
        cout << elem << "  ";
    }
    cout << endl;

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

