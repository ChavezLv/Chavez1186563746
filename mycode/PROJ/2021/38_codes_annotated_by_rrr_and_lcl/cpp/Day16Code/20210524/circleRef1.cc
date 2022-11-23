#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;

class Child;
//笔记by lcl:\
// shard_ptr的循环引用缺陷演示\

class Parent
{
public:
    Parent()
    {
        cout <<"Parent()" << endl;
    }

    ~Parent()
    {
        cout <<"~Parent()" << endl;
    }

    shared_ptr<Child> pPrent;
};

class Child
{
public:
    Child()
    {
        cout <<"Child()" << endl;
    }

    ~Child()
    {
        cout <<"~Child()" << endl;
    }

    shared_ptr<Parent> pChild;
};
//笔记by lcl:\
// shard_ptr的循环引用导致内存泄漏的缺陷演示\
1) 两个栈对象, 用智能指针创建, ppt里面有图示例, 此时打印use_count没有什么问题\
2)  parentPtr->pPrent = childPtr; //sp = sp\
     这时候 pPrent和ptrChild都指向堆区pChild\
    二者的引用计数都变成了2. 画图看得更清楚\
    childPtr->pChild = parentPtr;//sp = sp\
    此时\
    两个智能指针(栈对象) 执行析构函数时, ptrChild先销毁(后入栈)\
    ptrChild先脱离, 引用计数从2变为1, ptrParent后脱离, 引用计数也从2变为1\
    但是还有数据成员pParent(new Parent)与pChild(new Child)二者相互指引, 引用是1, 无法销毁\
3) 此时解决思路: 自动再减一, 或者两个指针指向同一个堆空间时不加1, 



void test()
{
    shared_ptr<Parent> parentPtr(new Parent());
    shared_ptr<Child> childPtr(new Child());
    cout << "parentPtr.use_count() = " << parentPtr.use_count() << endl;
    cout << "childPtr.use_count() = " << childPtr.use_count() << endl;

    parentPtr->pPrent = childPtr; // 这时候 pPrent和ptrChild都指向堆区pChild
    childPtr->pChild = parentPtr;
    cout << "parentPtr.use_count() = " << parentPtr.use_count() << endl;
    cout << "childPtr.use_count() = " << childPtr.use_count() << endl;
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

