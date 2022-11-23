#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

template<class T>
class Node {
public:
    T data;
    Node *pPre;
    Node *pNext;
};
template<class T>
class List{
public:
    List();
    ~List();

    void push_front(T data);//head insert
    void push_back(T data);//tail insert

    void pop_front();//head delete
    void pop_back();//tail delete

    bool find(T data);//
    void insert(T pos, T data);//insert data after pos
    void display() const;
    void erase(T data);//delete node.date  = data

private:
    Node<T> * _head;
    Node<T> * _tail;
    T _size;
};

template<class T>
List<T>::List(){
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}

template<class T>
List<T>::~List(){
    Node<T> * pCur = _head;
    Node<T> * pPre = _head;
    while(pCur){
        pCur = pCur->pNext;
        delete pPre;
        pPre = pCur;
    }
}


template<class T>
void List<T>::push_front(T data){
    Node<T>* pNew = new Node<T>();
    pNew->data = data;
    if(_head == nullptr || 0 == _size){
        _head = pNew;
        _tail = pNew;
        _head->pNext = nullptr;
        _tail->pNext = nullptr;
        _head->pPre = nullptr;
        _tail->pPre = nullptr;
    }else{
        pNew->pNext = _head;
        pNew->pPre = nullptr;
        _head = pNew;
    }    
    ++_size;
}


template<class T>
void List<T>::push_back(T data){
    Node<T>* pNew = new Node<T>();
    pNew->data = data;
    if(0 == _size){
        _head = pNew;
        _tail = pNew;
        _head->pNext = nullptr;
        _tail->pNext = nullptr;
        _head->pPre = nullptr;
        _tail->pPre = nullptr;
    }else{
        _tail->pNext = pNew;
        pNew->pPre = _tail;
        _tail = pNew;
        _tail->pNext = nullptr;
    }
    ++_size;
}

template<class T>
void List<T>::pop_front(){
    if(0 != _size){
        Node<T>* pCur = _head;
        _head = _head->pNext;
        _head->pPre = nullptr;
        delete pCur;
        pCur = nullptr;
        --_size;
        //如果此时_tail
        if(0 == _size){
            _tail = nullptr;
        }
    }
}

template<class T>
void List<T>::pop_back(){
    if(0 != _size){
        if(_tail == _head){
            delete _tail;
            _tail = _head = nullptr;
        }else{
            
            Node<T>* pCur = _tail;
            _tail = _tail->pPre;
            _tail->pNext = nullptr;//漏了
            delete pCur;
            pCur = nullptr;

        }

        --_size;
    }
}

template<class T>
bool List<T>::find(T data){
    Node<T>* pCur = _head;
    while(pCur){
        if(pCur->data == data){
            return true;
        }
        pCur = pCur->pNext;
    }
    return false;
}

template<class T>
void List<T>::insert(T pos, T data){
    Node<T>* pCur = _head;
    while(pCur){
        if(pCur->data == pos){
            if(pCur == _head){
                push_front(data);
                break;
            }
            //insert
            Node<T> *pNew = new Node<T>();
            pNew->data = data;
            pNew->pNext = nullptr;

            if(nullptr == pCur->pNext){
                pCur->pNext = pNew;
                pNew->pPre = pCur;
                _tail = pNew; 
            }else{
                pNew->pNext = pCur->pNext;
                pCur->pNext->pPre = pNew;
                pCur->pNext = pNew;
                pNew->pPre = pCur;
            }

            ++_size;
        }
        pCur = pCur->pNext;
    }

}

template<class T>
void List<T>::display()const{
    Node<T>* pCur = _head;
    while(pCur){
        cout << pCur->data << " ";
        pCur = pCur->pNext;
    }
    cout << endl;
}

template<class T>
void List<T>::erase(T data){
    Node<T>* pCur = _head;
    Node<T>* pPre = _head;
    while(pCur){
        if(pCur->data == data){
            //delete
            if(pCur == _head){
                pop_front();
                break;
            }else if(pCur == _tail){
                pop_back();
                break;
            }else{
                pPre->pNext = pCur->pNext;
                pCur->pNext->pPre = pPre;
                delete pCur;
                pCur = nullptr;

                --_size;
                break;
            }
        }
        pPre = pCur;
        pCur = pCur->pNext;
    }
}
int main()
{
    List<int> ls;
    for(int i = 0; i < 10; ++i){
        ls.push_back(i);
    }
    for(int i = 10; i < 20; ++i){
        ls.push_front(i);
    }
    ls.display();
    for(int i = 0; i < 10; i +=2){
        cout << i << endl;
        ls.erase(i);
    }
    ls.display();

    cout << ls.find(9) << endl;
    ls.pop_back();
    ls.display();

    ls.insert(10,100);
    ls.display();


    return 0;
}

