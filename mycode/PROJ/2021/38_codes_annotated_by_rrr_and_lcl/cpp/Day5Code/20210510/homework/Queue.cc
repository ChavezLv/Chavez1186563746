#include <iostream>

using std::cout;
using std::endl;

//对头指针_front与队尾指针_rear在进行变化的时候，需要考虑边界条件
class Queue
{
public:
    Queue(int sz = 10)
    : _size(sz)
    , _front(0)
    , _rear(0)
    , _data(new int[_size]())
    {
        cout << "Queue(int = 10)" << endl;
    }

    bool empty()
    {
        return _front == _rear;
    }

    bool full()
    {
        return _front == (_rear + 1)% _size;
    }

    void push(const int &value)
    {
        if(!full())
        {
            _data[_rear++] = value;
            _rear %= _size;
        }
        else
        {
            cout << "The queue is full" << endl;
            return;
        }
    }

    void pop()
    {
        if(!empty())
        {
            ++_front;
            _front %= _size;
        }
        else
        {
            cout << "The queue is empty" << endl;
            return;
        }
    }

    int front() const
    {
        return _data[_front];
    }

    int back() const
    {
        return _data[(_rear - 1 + _size)%_size];
    }

    int getFront() const
    {
        return _front;
    }

    int getRear() const
    {
        return _rear;
    }

    ~Queue()
    {
        cout << "~Queue()" << endl;
        if(_data)
        {
            delete [] _data;
            _data = nullptr;
        }
    }
private:
    int _size;
    int _front;
    int _rear;
    int *_data;
};
int main(int argc, char **argv)
{
    Queue que;
    cout << "此时队列是否为空?" << que.empty() << endl;
    que.push(1);
    cout << "此时队列是否为满?" << que.full() << endl;
    for(size_t idx = 2; idx != 12; ++idx)
    {
        que.push(idx);
    }
    cout << "此时队列是否为满?" << que.full() << endl;

    cout << "打印队列头尾" << endl << endl;
    cout << "队头元素"  << que.front() << endl;
    cout << "对头指针" << que.getFront() << endl;
    cout << "队尾元素"  << que.back() << endl;
    cout << "对尾指针" << que.getRear() << endl;

    while(!que.empty())
    {
        cout << que.front() << endl;
        que.pop();
    }
    cout << "此时队列是否为空?" << que.empty() << endl;
    return 0;
}

