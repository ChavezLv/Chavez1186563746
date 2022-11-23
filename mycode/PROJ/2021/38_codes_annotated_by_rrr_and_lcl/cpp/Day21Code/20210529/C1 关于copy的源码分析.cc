----------------------------------------Day21-------------------------------------
Trivial:平凡的、琐碎的
POD：plain old  data
    Trivial
    non_Trivial

ostream_iterator<int> osi(cout, "\n"); // 首先调用构造函数
源码: 
ostream_type* _M_stream;
const _CharT* _M_string;// ostream_iterator两个数据成员

//osi(cout, "\n")
__s = &cout;
__c = "\n";
ostream_iterator(ostream_type& __s, const _CharT* __c) 
: _M_stream(&__s), _M_string(__c) 
{
    
}


//#include<algorithm>
//先看没有两个下划线的算法
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result) {
  __STL_REQUIRES(_InputIter, _InputIterator);//check
  __STL_REQUIRES(_OutputIter, _OutputIterator);//check
  return __copy_aux(__first, __last, __result, __VALUE_TYPE(__first));
}

template <class _InputIter, class _OutputIter, class _Tp>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last,
                              _OutputIter __result, _Tp*) {
  typedef typename __type_traits<_Tp>::has_trivial_assignment_operator
          _Trivial;// typename, Trivial:平凡的、琐碎的, 区分代码, 类或者内置类型, 是类就构造, 内置内置就需要
  return __copy_aux2(__first, __last, __result, _Trivial());//
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux2(_InputIter __first, _InputIter __last,
                               _OutputIter __result, __false_type) {
  return __copy(__first, __last, __result,
                __ITERATOR_CATEGORY(__first),
                __DISTANCE_TYPE(__first));
}//


//以下是传参
/*
__first = number.begin();
__last = number.end();
__result = osi
*/
template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)
    *__result = *__first;//first != last就执行这句
  return __result;
}
f                 l
1  3   6   8   9   2
       f                l
1  3   6   8   9   2
    
    
*__result = 3
*osi = 3  (osi解引用是 return this; // 对象本身)
osi = 3
        ostream_iterator&
      operator*()
      { return *this; }
           
 //__result = osi类osi对象的赋值运算符函数   
 ostream_iterator<_Tp>& operator=(const _Tp& __value)
{ 
    *_M_stream << __value;//cout << 3
    if (_M_string) *_M_stream << _M_string;//cout << "\n"    _M_string 是指针, \n不等于空 
    return *this;
  }

//cout << 1
//cout << "\n" 
//cout << 3
//cout << "\n" 


  -------------------------------------Day21-----------------------------------


class istream_iterator
    : public iterator<input_iterator_tag, _Tp, _Dist, const _Tp*, const _Tp&>
    { ... }

//三个参数
istream_type* _M_stream;
 _Tp _M_value;// 接收值int ...
bool _M_ok;

// 到这里 istream_iterator<int> isi(std::cin);存了一个值, 但没有输入到vector, 需要通过copy
//isi(std::cin)  -----------------构造函数(0)---------------------
__s = cin;
istream_iterator(istream_type& __s) //构造函数
: _M_stream(&__s) 
{ 
    _M_read();
}

//--------------------(A)构造函数调用了一次----------------------
 void _M_read() {
    _M_ok = (_M_stream && *_M_stream) ? true : false;// 交集对_M_ok初始化
    if (_M_ok) {
      *_M_stream >> _M_value;//cin >>  _M_value  _M_value = 2
      _M_ok = *_M_stream ? true : false;   // _M_ok = 1
    }
  }
    
//--------------------------------解析完read退出----------------------------------------
__first = isi
__last =istream_iterator<int>()
__result = back_inserter(number)//_first, _last 拷贝到__result里面去
template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)// 首先比较是否相等, 然后解引用isi
    *__result = *__first;// --(*)isi-- 返回*__result = __M_value
    //*__result = 1; cin的1给了back_inserater
    // 看--- back_inserter(1)--(2), 然后要解引用这个类看(3)解引用*, (4)=, 第一个元素1插入完成!!!
    
    
    //然后++__result, 看(3) 还是返回对象本身, ++first的前置isi的前置++ 看(a), 就是前置++这里卡住了一下, 只有ctrl + d才会退出, 如果这时候, 进入到_M_read() cin >> ctrl+d, 流对象的状态发生了变化, 就会退出来, cin往_M_value输东西就是int类型的
  return __result;
}

    
    //--------------------(*)isi-------------------
        const _Tp&
      operator*() const
      {
  __glibcxx_requires_cond(_M_ok,
        _M_message(__gnu_debug::__msg_deref_istream)
        ._M_iterator(*this));
  return _M_value;//*__result = __M_value -> *__result = 1; 
      }

  // ++ isi ------------ (a) ---------------------
  // ++first的前置isi的前置++
    //前置++, 插入操作在前置++这里完成
    istream_iterator& operator++() { 
    _M_read(); //里面调用了_M_read(); 看(A) , 继续是true
    return *this; //前置++也是返回*this
        //总结: 前置++ 也相当于进行了输入操作? 这里一次插入(cin) 然后真正插入的时候就是 在copy里面(push_back)
  }
// ---------------- back_inserter(4)-----------------
// 最后通过copy在底层一系列的调用之后还是调用赋值运算符函数 value_type = int
//  const typename _Container::value_type类型萃取
back_insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    container->push_back(__value);// 还是把数据插入到number里面取了, 同理, front_iterator和insert_iterator就是调用的push_front和insert
    return *this;
  }


// ---------------- back_inserter(1)-----------------
template <class _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __x) {
  return back_insert_iterator<_Container>(__x);
}
// ---------------- back_inserter(2)-----------------
// _Container& __x = (vector)number
back_insert_iterator(_Container& __x) 
: container(&__x) 
{
    
}
// ---------------- back_inserter(3)-----------------
// 解引用, 返回对象本身,  *__result = 1;  __result = 1; 去调用赋值运算符函数
back_insert_iterator<_Container>& operator*() { return *this; }
back_insert_iterator<_Container>& operator++() { return *this; }
back_insert_iterator<_Container>& operator++(int) { return *this; }