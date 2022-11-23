 
#include <iostream>
#include <functional>
using namespace std;
 
class View
{
public:
    void onClick(int x, int y)
    {
        cout << "X : " << x << ", Y : " << y << endl;
    }
    void func1()
    {   int y=4,x=2;
        cout << "X : " << x << ", Y : " << y << endl;
    }
    void func2()
    {   int y=444,x=222;
        cout << "X : " << x << ", Y : " << y << endl;
    }

};
 
// 定义function类型, 三个参数
function<void(View, int, int)> clickCallback;
function<void(View)> Func1;
function<void()> Func2;
 
int main(int argc, const char * argv[])
{
    View button;
 
    // 直接通过 类 获取到代码区的成员函数入口地址;
    clickCallback = &View::onClick;
    //实例化之后的传参
    clickCallback(button, 10, 123);
    
    //通过类
    Func1=&View::func1;
    Func1(button);
    
    //通过对象获取函数指针
    Func2=std::bind(&View::func2,&button);
    Func2();

    return 0;
}
