#include <iostream>

using std::cout;
using std::endl;

//将构造函数用protected修饰，那么该类就是抽象类（抽象类的第二种形式）
//注意：这个与把构造函数设置为私有的不同
class Base
{
protected:
/* private: */
    Base(long base = 0)
    : _base(base)
    {
        cout << "Base(long = 0)" << endl;
    }
private:
    long _base;
};

class Derived
: public Base
{
public:
    Derived(long base = 0, long derived = 0)
    : Base(base)
    , _derived(derived)
    {
        cout << "Derived(long = 0, long = 0)" << endl;
    }
private:
    long _derived;
};
int main(int argc, char **argv)
{
    /* Base base(10);//error */
    return 0;
}

