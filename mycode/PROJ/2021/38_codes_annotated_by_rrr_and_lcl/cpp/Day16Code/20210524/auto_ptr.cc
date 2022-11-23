#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::auto_ptr;


void test()
{
    //原生指针（裸指针）
    int *pInt = new int(10);
    auto_ptr<int> ap(pInt);//auto_ptr有缺陷 , 另外三种的提出就是弥补这个的缺陷
    cout << "*ap = " << *ap << endl;
    cout << "ap.get() = " << ap.get() << endl;//(1)
    cout << "pInt = " << pInt << endl;//(2)与(1)打印的地址值相等

    cout << endl;
    auto_ptr<int> ap2(ap);//表面上执行了拷贝构造函数，但是底层已经
                         //发生了所有权的转移，这个智能指针存在缺陷
    cout << "*ap2 = " << *ap2 << endl;
    cout << "*ap = " << *ap << endl;
    cout << "1111" << endl;
    
    //笔记by lcl:
    //(1) 为什么可以写成"auto_ptr<int> ap(pInt);" , 因为使用了模板, \
    可对比RAII.cc来理解.\
    一样的写法\
    "RAII<int> raii(new int(10));"\
    源于构造函数RAII(T *data): ...{}
    //(2) 为什么ap对ap2进行初始化之后, 就变成空了?\
    看下面的源码source(sgi_stl的代码)可知: \
          element_type*\
      release() throw()\
      {\
    element_type* __tmp = _M_ptr;\
    _M_ptr = 0;\
    return __tmp;\
      }\
        这就是缺陷所在!
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

