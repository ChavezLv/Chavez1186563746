#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

#define N 10

class Stack {
public:
    Stack(){
        memset(m_arr, 0, sizeof(m_arr));
        m_head = -1;
    }

    void push(int node);

    void pop();

    int top();

    bool empty();

    bool full();
        
private:
    int m_arr[N];
    int m_head;
    
};

void Stack::push(int node){
    if(m_head < N - 1){
        ++m_head;
        m_arr[m_head] = node;
    }
}

void Stack::pop(){
    if(Stack::empty()){
        cout << "empty stack" << endl;
    }else{
        cout << "pop: " << m_arr[m_head] << endl;
        --m_head;
    }
}

int Stack::top(){ 
    return m_arr[m_head];
}

bool Stack::empty(){
    if(-1 == m_head){
        return true;
    }else{
        return false;
    }
}

bool Stack::full(){
    if(m_head == N - 1){
        return true;
    }else{
        return false;
    }
}
int main()
{
    Stack sta;
    cout << sta.empty() << endl;
    cout << sta.full() << endl;
    for(int i = 0; i < N; ++i){
        sta.push(i);
        /* cout << sta.top() << endl; */
    }
    cout << sta.empty() << endl;
    cout << sta.full() << endl;
    
    for(int i = 0; i < N; ++i){
        sta.pop();
        /* cout << sta.top() << endl; */
    }
    return 0;
}

