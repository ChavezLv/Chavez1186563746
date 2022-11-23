#include <string.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;
using std::shared_ptr;


#define N 10

template<typename Type>
class Queue {
public:
    Queue();
    void push(Type node);
    void pop();
    Type front();
    Type back();
    bool empty();
    bool full();

private:
    Type m_arr[N];
    int m_front;
    int m_rear;

};
template<typename Type>
Queue<Type>::Queue(){
    memset(&m_arr, 0, sizeof(m_arr));
    m_front = 0;
    m_rear = 0;

}

template<typename Type>
void Queue<Type>::push(Type node){
    if(!full()){ 
        m_arr[m_rear] = node;
        m_rear = (m_rear + 1) % N;

    }

}

template<typename Type>
void Queue<Type>::pop(){
    if(!empty()){
        /* cout << m_arr[m_front] << endl; */
        memset(&m_arr[m_front], 0, sizeof(m_arr[m_front]));
        m_front = (m_front + 1) % N;

    }

}

template<typename Type>
Type Queue<Type>::front(){
    return m_arr[m_front];

}

template<typename Type>
Type Queue<Type>::back(){
    return m_arr[m_rear];

}


template<typename Type>
bool Queue<Type>::empty(){
    if(m_front == m_rear){
        return true;

    }else{
        return false;

    }

}

template<typename Type>
bool Queue<Type>::full(){
    if((m_rear + 1) % N == m_front){
        return true;

    } 
    return false;

}

void test(){
    Queue<shared_ptr<int>> que;
    cout << "empty? " << que.empty() << endl;
    cout << "full? " << que.full() << endl;
    for(int i = 1; i < N; ++i){
        que.push(shared_ptr<int>(new int(i)));
    }
    cout << "after push" << endl;
    cout << "empty? " << que.empty() << endl;
    cout << "full? " << que.full() << endl;

    for(int i = 0; i < N; ++i){
        cout << que.front() << " ";
        if(que.front()){
            cout << *que.front() << " / ";//print when not null
        }
        que.pop();
    }
    cout << endl;
    cout << "after pop" << endl;
    cout << "empty? " << que.empty() << endl;
    cout << "full? " << que.full() << endl;
}

void test1(){
    Queue<std::string> que;
    cout << "empty? " << que.empty() << endl;
    cout << "full? " << que.full() << endl;
    for(int i = 1; i < N; ++i){
        que.push(std::to_string(i));
    }
    cout << "after push" << endl;
    cout << "empty? " << que.empty() << endl;
    cout << "full? " << que.full() << endl;

    for(int i = 0; i < N; ++i){
        cout << que.front() << " ";
        que.pop();
    }
    cout << endl;
    cout << "after pop" << endl;
    cout << "empty? " << que.empty() << endl;
    cout << "full? " << que.full() << endl;
}

int main()
{
    test();
    test1();
    return 0;

}
