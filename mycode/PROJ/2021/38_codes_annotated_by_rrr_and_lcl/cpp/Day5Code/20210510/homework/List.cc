#include <iostream>

using std::cout;
using std::endl;

struct Node
{
    Node(int data = 0, Node *pre = nullptr, Node *next = nullptr)
    : _data(data)
    , _pre(pre)
    , _next(next)
    {
        cout << "Node(int = 0, Node * = nullptr, Node * = nullptr)" << endl;
    }

    ~Node()
    {
        cout << "~Node()" << endl;
    }
    int _data;
    Node *_pre;
    Node *_next;
};

class List
{
public:
    List()
    : _head(new Node())
    , _tail(new Node())
    , _size(0)
    {
        cout << "List()" << endl;
        _head->_next = _tail;
        _tail->_pre = _head;
    }

    //在链表的头部进行插入
    void push_front(int data)
    {
        Node *newNode = new Node(data);
        newNode->_next = _head->_next;
        newNode->_pre = _head;
        _head->_next->_pre = newNode;
        _head->_next = newNode;

        ++_size;
    }

    //在链表的尾部进行插入
    void push_back(int data)
    {
        Node *newNode = new Node(data);
        newNode->_next = _tail;
        newNode->_pre = _tail->_pre;
        _tail->_pre->_next = newNode;
        _tail->_pre = newNode;

        ++_size;
    }

    //在中间进行插入
    void insert(int pos, int data)
    {
        if(pos < 0 || pos > size())
        {
            cout << "该位置异常，无法插入 " << endl;
            return;
        }
        Node *pNode = _head;
        while(pos-- > 0)
        {
            pNode = pNode->_next;
        }

        Node *newNode = new Node(data);

        newNode->_next = pNode->_next;
        newNode->_pre = pNode;
        pNode->_next->_pre = newNode;
        pNode->_next = newNode;

        ++_size;
    }

    //在中间进行删除
    void erase(int data)
    {
        if(size() > 0)
        {
            bool flag = find(data);
            if(flag)
            {
                Node *pNode = _head->_next;
                while(pNode && pNode != _tail)
                {
                    if(data == pNode->_data)
                    {
                        Node *ptmp = pNode;
                        /* pNode = pNode->_next; */
                        /* ptmp->_pre->_next = pNode; */
                        /* pNode->_pre = ptmp->_pre; */
                        pNode = pNode->_pre;
                        ptmp->_next->_pre = pNode;
                        pNode->_next = ptmp->_next;


                        delete ptmp;
                        ptmp = nullptr;
                        --_size;
                    }
                    pNode = pNode->_next; 
                }

            }
            else
            {
                cout << "该元素不在链表中，无法删除 " << endl;
                return;
            }
        }
        else
        {
            cout << "该链表为空，无法进行删除 " << endl;
            return;
        }
    }

    //在链表的头部进行删除
    void pop_front()
    {
        if(size() > 0)
        {
            Node *pNode = _head->_next;
            pNode->_next->_pre = _head;
            _head->_next = pNode->_next;

            delete pNode;
            pNode = nullptr;
            --_size;
        }
        else
        {
            cout << "该链表为空，无法在头部进行删除" << endl;
            return;
        }
    }
    
    //在链表的尾部进行删除
    void pop_back()
    {
        if(size() > 0)
        {
            Node *pNode = _tail->_pre;
            pNode->_pre->_next = _tail;
            _tail->_pre = pNode->_pre;

            delete pNode;
            pNode = nullptr;
            --_size;
        }
        else
        {
            cout << "该链表为空，无法在头部进行删除" << endl;
            return;
        }
    }

    void printList() const
    {
        if(size() > 0)
        {
            Node *pNode= _head->_next;
            while(pNode && pNode != _tail)
            {
                cout << pNode->_data << "  ";
                pNode = pNode->_next;
            }
            cout << endl;
        }
        else
        {
            cout << "该链表为空，无法打印" << endl;
            return;
        }

    }

    bool find(int data)
    {
        if(size() > 0)
        {
            Node *pNode = _head->_next;
            while(pNode && pNode != _tail)
            {
                if(data == pNode->_data)
                {
                    return true;
                }
                pNode = pNode->_next;
            }

            return false;
        }
        else
        {
            cout << "该链表为空，无法查找元素" << endl;
            return false;
        }
    }

    int size() const
    {
        return _size;
    }

    ~List()
    {
        cout << "~List()" << endl;
        Node *deleteNode = _head->_next;
        while(deleteNode)
        {
            Node *nextNode = deleteNode->_next;
            delete deleteNode;
            deleteNode = nullptr;
            deleteNode = nextNode;
        }
        delete _head;
        _head = nullptr;
    }
private:
    Node *_head;
    Node *_tail;
    int _size;
};
int main(int argc, char *argv[])
{
    List lst;
    lst.printList();
    lst.erase(100);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;

    cout << endl << "在链表的头部进行插入 :" << endl;
    lst.push_front(1);
    lst.push_front(3);
    lst.push_front(5);
    lst.printList();

    cout << endl << "在链表的尾部进行插入 :" << endl;
    lst.push_back(11);
    lst.push_back(33);
    lst.push_back(55);
    lst.printList();

    cout << endl << "在链表的头部进行删除 :" << endl;
    lst.pop_front();
    lst.printList();

    cout << endl << "在链表的尾部进行删除 :" << endl;
    lst.pop_back();
    lst.printList();

    bool flag = lst.find(33);
    cout << "flag = " << flag << endl;

    cout << endl << "在中间某个位置删除" << endl;
    lst.insert(2, 100);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;

    lst.erase(11);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;

    lst.erase(1);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;

    lst.erase(3);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;

    lst.erase(33);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;

    cout << "12345" << endl;
    lst.erase(100);
    lst.printList();
    cout << "size = " << lst.size() << endl;
    cout << endl;
    return 0;
}

