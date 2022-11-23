#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

template<class Type>
class Node{
public: 
/* private: */
    Type m_data;
    Node* m_pNext;
};

template<class Type>
class Stack {
public:
   Stack();
   Stack(Type a[], int n);
   ~Stack();

   void push(Type data);
   void pop(Type &data);

   int top();

   bool empty();
   bool full();


private:
    Node<Type> *m_head;
};

template<class Type>
Stack<Type>::Stack(){
    m_head = new Node<Type>();
    m_head->m_pNext = nullptr;
}

template<class Type>
Stack<Type>::Stack(Type a[], int n){
    m_head = new Node<Type>();
    m_head->m_pNext = nullptr;
    //带有头结点的链表
    for(int i = 0; i < n; ++i){
        Node<Type> *p = new Node<Type>();
        p->m_data = a[i];
        p->m_pNext = m_head->m_pNext;
        m_head->m_pNext = p;
    }

}

template<class Type>
Stack<Type>::~Stack(){
    Node<Type> *p = m_head->m_pNext;
    Node<Type> *q = nullptr;
    while(p != nullptr){
        q = p->m_pNext;
        delete p;
        p = q;
    }
    //有必要吗?
    delete m_head;
    m_head = nullptr;
}

int main()
{
    
    return 0;
}

