#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
//course2 
// 笔记by lcl:\
1)template <typename T, size_t kSize = 10> 用非类型参数传入栈的大小\
\
2)类名+作用域限定符, 用于在类外面实现时的写法\
template <typename T, size_t kSize>\
Stack<T, kSize>::~Stack()\
模板形式, 此时类是一个抽象类型, 不知道是什么类型, 则必须要把T, ksize传入, 写明\
而这个T, ksize也不知道这是哪里来的,\
则必须要写明, template<typename T, size_t kSize>\
\
3) 成员模板不能声明为virtual, 类里面的函数, stack是抽象, 编译时一个个具体化出来, 但是\
又想着运行时实现多态, 这就是矛盾的! \
\
4) 模板的嵌套(大概了解了一下, 在类外面实现的时候,因为是抽象类, 也要分层一个个实现出来)\
5) 模板做参数(模板的参数可以时另外一个模板), 知道就可以了\
 template< template<typename _Tp1>\
            typename T2, typename T3) \
            template<template <class T1> class T2, class T3,int Num> //PPT有例子



//类模板
template <typename T, size_t kSize = 10>
class Stack
{
public:
    Stack()
    : _top(-1)
    , _data(new T[kSize]())
    {

    }
    ~Stack();
    bool empty() const;
    bool full() const;
    void push(const T &t);
    void pop();
    T top() const;
private:
    int _top;
    T *_data;
};

template <typename T, size_t kSize>
Stack<T, kSize>::~Stack()
{
    if(_data)
    {
        delete [] _data;
        _data = nullptr;
    }
}

template <typename T, size_t kSize>
bool Stack<T, kSize>::empty() const
{
    return -1 == _top;
}

template <typename T, size_t kSize>
bool Stack<T, kSize>::full() const
{
    return _top == kSize - 1;
}

template <typename T, size_t kSize>
void Stack<T,kSize>::push(const T &t)
{
    if(!full())
    {
        _data[++_top] = t;
    }
    else
    {
        cout << "The stack is full" << endl;
        return;
    }
}

template <typename T, size_t kSize>
void Stack<T, kSize>::pop()
{
    if(!empty())
    {
        --_top;
    }
    else
    {
        cout << "The stack is empty" << endl;
        return;
    }
}
template <typename T, size_t kSize>
T Stack<T, kSize>::top() const
{
    return _data[_top];
}

void test()
{
    Stack<int, 15> st;
    cout << "栈是不是空的? " << st.empty() << endl;
    st.push(1);
    cout << "栈是不是满的? " << st.full() << endl;

    for(size_t idx = 2; idx != 20; ++idx)
    {
        st.push(idx);
    }

    while(!st.empty())
    {
        cout << st.top() << "  ";
        st.pop();
    }
    cout << endl;
    cout << "栈是不是空的? " << st.empty() << endl;
}

void test2()
{
    Stack<string> st;
    cout << "栈是不是空的? " << st.empty() << endl;
    st.push(string("aa"));
    cout << "栈是不是满的? " << st.full() << endl;

    for(size_t idx = 1; idx != 20; ++idx)
    {
        st.push(string(2, 'a' + idx));
    }

    while(!st.empty())
    {
        cout << st.top() << "  ";
        st.pop();
    }
    cout << endl;
    cout << "栈是不是空的? " << st.empty() << endl;
}
int main(int argc, char **argv)
{
    test2();
    return 0;
}

