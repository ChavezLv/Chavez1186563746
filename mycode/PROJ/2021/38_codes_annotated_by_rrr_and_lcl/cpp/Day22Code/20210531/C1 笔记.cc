---------------------------------day22---------------------------------------
//first = number.begin();
//last = number.end();
//p = display
bool display(int value)
{  
     return value > 5;
 }

template<class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
    first = std::find_if(first, last, p);//查找第一个满足条件p的迭代器, 
    //第一个就是p(7)返回true时候的返回
    if (first != last)
    {
        // first 赋值给 i
         for(ForwardIt i = first; ++i != last; )
         {
             if (!p(*i))// p(7)取反进不去,然后再++
             {
                  *first++ = std::move(*i);// 内置类型std::move无效果
                 // 目的: 加快效率, 比如当非内置类型时
                 
                 //下一个是4, 赋值给7的位置, 所以7变成了4. f++
                 
             }
               
         }
            
    }
       
    return first;
}
1, 3, 5, 7, 4, 3, 2, 6, 8, 9, 1
    
                     f           l
1, 3, 5, 4, 3, 2, 1, 6, 8, 9, 1
                                 i  (i!=last了, 返回first, 待删除元素的首地址), 所以就这样打印出来了)
    
    
template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first) 
    {
        if (p(*first)) {// 第一次返回false, 因为是1 , 直到  p(7)返回true, 
            return first;// p(7) 返回 7
        }
    }
    return last;
}
---------------------------------day22---------------------------------------


value > 5

bind1st(F, 5)
F(5, number.begin(), number.end())
std::less(5, Y)
    
    
bind2nd(F, 5)
F(number.begin(), number.end(), 5)
std::greater(Y, 5)
---------------------------------day22---------------------------------------
空间配置器allocator

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