#include <iostream>
#include <memory>
/* using namespace std; */
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

template<typename Type>
class Node{
public: 
    /* private: */
    int m_size;
    Type m_data;
    Node* m_pNext;

};

template<typename Type>
class Stack {
#define CAPACITY 100
public:
    Stack();
    Stack(Type a[], int n);
    ~Stack();

    void push(Type data);
    void pop();

    Type top();

    bool empty();
    bool full();


private:
    Node<Type> *m_head;

};

template<typename Type>
Stack<Type>::Stack(){
    m_head = new Node<Type>();
    m_head->m_size = 0;
    m_head->m_pNext = nullptr;

}

template<typename Type>
Stack<Type>::Stack(Type a[], int n){
    m_head = new Node<Type>();
    m_head->m_size = n;
    m_head->m_pNext = nullptr;
    
    for(int i = 0; i < n; ++i){
        Node<Type> *p = new Node<Type>();
        p->m_data = a[i];
        p->m_pNext = m_head->m_pNext;
        m_head->m_pNext = p;
    }

}

template<typename Type>
void Stack<Type>::push(Type data){
    if(full()){
        cout << "full!" << endl;
        return;
    }
    Node<Type> *pNew = new Node<Type>();
    pNew->m_data = data;
    if(nullptr == m_head){
        m_head->m_pNext = pNew;    
        pNew->m_pNext = nullptr;
    }else{
        pNew->m_pNext = m_head->m_pNext;
        m_head->m_pNext = pNew;
    }
    ++ m_head->m_size;
}
template<typename Type>
void Stack<Type>::pop(){
    if(!empty()){
        Node<Type> *p = m_head->m_pNext;
        m_head->m_pNext = p->m_pNext;
        delete p;
        p = nullptr;
        -- m_head->m_size;
    }
}

template<typename Type>
Type Stack<Type>::top(){
    if(empty()){
        cout << "empty stack!" << endl;
        return m_head->m_data;
    }
    return m_head->m_pNext->m_data;
}


template<typename Type>
bool Stack<Type>::empty(){
    return m_head->m_size == 0;
}

template<typename Type>
bool Stack<Type>::full(){
    return m_head->m_size == CAPACITY;
}

template<typename Type>
Stack<Type>::~Stack(){
    Node<Type> *p = m_head->m_pNext;
    Node<Type> *q = nullptr;
    while(p != nullptr){
        q = p->m_pNext;
        delete p;
        p = q;
    }

    delete m_head;
    m_head = nullptr;
}

void test(){
    /* Stack<int> sk; */
    Stack<shared_ptr<int>> sk; 
    sk.push(shared_ptr<int>(new int(10)));
    sk.push(shared_ptr<int>(new int(100)));
    sk.pop();
    cout << *sk.top() << endl;
    
}
int main()
{
    test();
    return 0;
}
