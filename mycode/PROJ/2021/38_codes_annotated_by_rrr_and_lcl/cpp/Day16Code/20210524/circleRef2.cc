#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;

class Child;

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

    weak_ptr<Parent> pChild;
};

void test()
{
    shared_ptr<Parent> parentPtr(new Parent());
    shared_ptr<Child> childPtr(new Child());
    
    cout << "parentPtr.use_count() = " << parentPtr.use_count() << endl;
    cout << "childPtr.use_count() = " << childPtr.use_count() << endl;

    parentPtr->pPrent = childPtr; 
    childPtr->pChild = parentPtr;
    cout << "parentPtr.use_count() = " << parentPtr.use_count() << endl;//1
    cout << "childPtr.use_count() = " << childPtr.use_count() << endl;//2
}
// 笔记by lcl:\
解决智能指针循环引用的销毁问题的解决方案: 弱引用\
只改了一个 40 行的weak_ptr\
parentPtr->pPrent = childPtr; //sp = sp ,引用计数变为2\
childPtr->pChild = parentPtr;//wp = sp, 引用计数不会增加\
先是ptrChild脱离, 指向pChild的引用计数从2变为1, ptrParent脱离时, 引用计数变为0, 析构函数执行, \
pPrent干掉, 则指向pChild的引用计数从1变为0\
如上, 可见, 先创建先销毁, 有点违背常识?  实际就是因为引用计数

int main(int argc, char **argv)
{
    test();
    return 0;
}

