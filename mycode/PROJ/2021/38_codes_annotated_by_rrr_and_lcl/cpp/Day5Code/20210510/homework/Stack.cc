#include <iostream>

using std::cout;
using std::endl;
const int MAXSIZE = 10;
class Stack
{
public:
    Stack(int sz = 10)
    : _size(sz)
    , _top(-1)
    , _data(new int[_size]())
    {
        cout << "Stack(int = 10)" << endl;
    }

    bool empty()
    {
#if 0
        if(-1 == _top)
        {
            return true;
        }
        else
        {
            return false;
        }
#endif
        return (-1 == _top);
    }

    bool full()
    {
        return (_top == _size - 1);
    }

    void push(const int &value)
    {
        if(!full())
        {
            _data[++_top] = value;
        }
        else
        {
            cout << "The stack is full, can not push any data" << endl;
        }
    }
    void pop()
    {
        if(!empty())
        {
            --_top;
        }
        else
        {
            cout << "The stack is empty" << endl;
        }
    }

    int top()
    {
        return _data[_top];
    }

    ~Stack()
    {
        cout << "~Stack()" << endl;
        if(_data)
        {
            delete [] _data;
            _data = nullptr;
        }
    }
private:
    int _size;
    int _top;
    /* int _data[MAXSIZE] = {0}; */
    int *_data;
};
int main(int argc, char **argv)
{
    Stack st;
    cout << "该栈是不是空的？" << st.empty() << endl;
    st.push(1);
    cout << "该栈是不是满的？" << st.full() << endl;

    for(size_t idx = 2; idx != 12; ++idx)
    {
        st.push(idx);
    }
    cout << "该栈是不是满的？" << st.full() << endl;

    while(!st.empty())
    {
        cout << st.top() << endl;
        st.pop();
    }
    cout << "该栈是不是空的？" << st.empty() << endl;

    return 0;
}

