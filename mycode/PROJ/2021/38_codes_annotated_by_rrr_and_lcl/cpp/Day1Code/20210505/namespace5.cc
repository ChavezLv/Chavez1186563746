#include <iostream>

using std::cout;
using std::endl;

//标准命名空间可以进行扩展
//有可能会与std中的实体冲突，一般不建议进行扩展(标准命名空间中的实体一般都是小写)
namespace  std
{
struct MyStruct
{
    int ia = 0;
};
}//end of namespace std

#if 0
//结构体是不能进行扩展的
struct A
{
int ia;
};

struct A
{
int ib;

};
#endif
//命名空间是可以进行扩展的
//带命名空间的函数声明
namespace  wd
{
void print();
}

namespace hb
{
int number = 20;

void show()
{
    cout << "void show()" << endl;
}

void display()
{
    cout << "void display()" << endl;
    wd::print();
}

}//end of namespace hb

namespace wd
{
int number = 10;

void print()
{
    cout << "void print()" << endl;
    hb::show();
}

}//end of namespace

int main(int argc, char **argv)
{
    cout << "number = " << wd::number << endl;//作用域限定符
    wd::print();
    return 0;
}

