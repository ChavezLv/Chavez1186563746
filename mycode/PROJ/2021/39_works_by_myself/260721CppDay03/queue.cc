#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;


#define N 10
class Queue {
public:
    Queue();
    void push(int node);
    void pop();
    int front();
    int back();
    bool empty();
    bool full();

private:
    int m_arr[N];
    int m_front;
    int m_rear;
};

Queue::Queue(){
    memset(&m_arr, 0, sizeof(m_arr));
    m_front = 0;
    m_rear = 0;
}

void Queue::push(int node){
    if(!full()){ 
        m_arr[m_rear] = node;
        m_rear = (m_rear + 1) % N;
    }
}

void Queue::pop(){
    if(!empty()){
        cout << m_arr[m_front] << endl;
        m_arr[m_front] = 0;
        m_front = (m_front + 1) % N;
    }
}

int Queue::front(){
    return m_arr[m_front];
}

int Queue::back(){
    return m_arr[m_rear];
}

bool Queue::empty(){
    if(m_front == m_rear){
        return true;
    }else{
        return false;
    }
}

bool Queue::full(){
    if((m_rear + 1) % N == m_front){
        return true;
    } 
    return false;
}
int main()
{
    Queue que;
    cout << que.empty() << endl;
    cout << que.full() << endl;
    for(int i = 0; i < N; ++i){
        que.push(i);
    }
    cout << que.empty() << endl;
    cout << que.full() << endl;
    return 0;
}

