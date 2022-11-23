#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
    //一般只有一个参数的这种构造,要加一个explicit
    /*explicit*/
    Base(long member = 0)
    : _member(member)
    {
        cout << "Base(long = 0) " << endl;
    }

    virtual
    void print()
    {
        cout << "Base::_member = " << _member << endl;
    }
/* private: */
protected:
    long _member;
};

class Derived
: public Base
{
public:
    Derived(long member1 = 0, long member2 = 0)
    : Base(member1)
    , _member(member2)
    {
        cout << "Derived(long = 0)" << endl;
    }

    /* void print(); */
    virtual
    /* void print() */
    void print(int x)
    {
        /* cout << "x = " << x << endl; */
        cout << "Base::_member = " << Base::_member << endl;
        cout << "_member = " << _member << endl;
    }
private:
    long _member;
};
int main(int argc, char **argv)
//从作用域上区分重载, 再从参数列表细分出覆盖
{   //隐藏的特点: 跟是否是虚函数无关. 要看是 同名函数+不同参数列表. 
    //如果连参数列表也是相同的, 则也满足覆盖, 走的是多态路线. 
    //即覆盖是特殊的隐藏

    //隐藏的两种形式:
    //如果基类和派生类的数据成员也是同名的, 在派生类中, 基类的数乘以也会被隐藏.
    Base base(10);
    Derived derived(20, 30);
    /* derived.print();//error, 基类的同名函数被隐藏了 */
    derived.print(1);
    /* derived.Base::print(); //虽然是隐藏,但不是直接消失了. 要直接使用基类的作用域限定符才能访问到*/
    /*如果基类生成一个对象，然后用派生类的对象赋值给基类对象，那么基类的_base会发生变化吗？￼
*/
    return 0;
}

