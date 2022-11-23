#include <iostream>
#include <functional>

using std::cout;
using std::endl;
using std::bind;
//course1 
// 笔记by lcl:
/*

--------------bind1st和bind2nd的升级版------bind-------------------------

1) 可以绑定n元函数的n个参数, 究极强
2) 也可以绑定类的成员函数
但此时!  必须要注意成员函数是几个参数的? this 传对象

auto f1 = bind(&Test::add, &tst, 4, 5); 

add函数名 + 限定符 + 取地址


3) 深度思考:   面向对象的角度去理解所有东西? 

typedef int (*pFunc)();

pFunc f = func1;//函数指针创建的对象
f(); //也像  函数对象的形式
如:
void test2()
{
    pFunc f = &func2;//注册回调函数,钩子函数
    cout << "f() = " << f() << endl;//执行回调函数

    f = func3;
    cout << "f() = " << f() << endl;
}
上述: func2 func3 在程序代码区是有地址的, 就可以看成有地址的变量
有地址的对象, 同一个指针, 不同对象, 产生不同的行为


上面就是函数指针的用法, c++里面有个概念: 多态

函数指针实现多态
比如pFunc看成对象指针, 指向多个不同对象

所以c++的多态 = c语言的struct + 函数指针 实现的多态

4) 回调函数:  函数的注册与函数的执行分开, 就是函数指针这里
pFunc f = &func2;// 钩子函数, 只要不去频繁变动接受不同对象, 就属于回调函数的含义
钩子函数(工作中c相关的称呼) : 用f去钩 某种类型的函数

回忆: pthread_create的第三个参数, 也是函数指针
回调函数也是相同的形式


5) 即学即用: ---bind与函数指针的混用-----------------------
函数指针 每次不同形式都要写一个typedef


*/
// --------------------------courser3-test3()-------------------------
/*
万物对象  ,  所有函数都是函数对象, 函数指针也是, 重载了()的类也是

为什么bind比函数指针更灵活些?
占位符

1) 什么是占位符? 
void func5(int x1, int x2, int x3, const int &x4, int x5){//...}
int number = 300;

auto f = bind(func5, 10, std::placeholders::_5, 
                  std::placeholders::_3, std::cref(number), number);

f(1, 3, 5, 7, 9);// 传的参数, 像1, 3, 7, 只起到占位置的作业
//但是传参要最起码大于占位符的最大值-->  _5



//打印: 10, 9, 5, 300, 300  //参数2的位置, 接收的是传入的5

2) 特点: 
    //占位符本身表明的是形参的位置
    //占位符中数字参数代表函数传递进来的第几个参数
    //bind的传输的传递是值传递, 所以要用cref

    //cref =  const reference,引用的包装器
    // ref = 普通的引用

-------------------再回到test()---------



--------------------开始讲解 function----------------
函数也是有类型的, 再找一个容器装类型  比如int() 就是一种类型


function本身是一个类, 类模板 : 
template< class R, class... Args >
class function<R(Args...)>;

这样就可以改一下开头的auto这里了
1) 

std::function<int()> f = bind(add, 1, 4);// 返回类型是int参数无参
类比: vector<int> 

推导的时候, 占位符最大值也会影响到参数个数, 没填对就会报错

2) ----------------bind 还能绑定数据成员--------------------
补充一下:  c++11数据成员初始化的方式, 可以不放在初始化列表里面, 有些编译器不支持

但一般还是要放在初始化列表里面比较好

int data = 100; //c++11数据成员初始化的方式

f2 = bind(&Test::data, &tst); 
因为这里data被理解为 返回是int,参数是无参的
比如string也是同理. 

小小总结: bind灵活到可以绑定数据类型....

--------------跳转figure.cc----------------


*/

int add(int x, int y)
{
    cout << "int add(int, int)" << endl;
    return x + y;
}

int multiply(int x, int y)
{
    return x * y;
}

class Test
{
public:
    int add(int x, int y)
    {
        cout << "int Test::add(int, int)" << endl;
        return x + y;
    }

    static 
    int func(int x)
    {
        return x;
    }

    int data = 100;//可以进行初始化

};
void test()
{
    //int (int, int)    ====> int ()
    //int ()
    /* auto f = bind(add, 1, 4); */
    //函数的容器
    std::function<int()> f = bind(add, 1, 4);// bind绑定了add的两个参数!
    cout << "f() = " << f() << endl;

    Test tst;
    //int ()
    auto f1 = bind(&Test::add, &tst, 4, 5); 
    cout << "f1() = " << f1() << endl;

    /* auto f2 = bind(&Test::func, 6); */
    std::function<int()> f2 = bind(&Test::func, 6);
    cout << "f2() = " << f2() << endl;


    cout << endl;
    f2 = bind(&Test::data, &tst); 
    //[r]函数静态成员的地址(指针)，可以被当做【callable object】，作为函数调用
    //[R2]而且【不是无参】，【有一个参数】，this指针
    
    cout << "f2(100) = " << f2() << endl;


    //占位符, 结果就是一个参数都没有固定
    //int (int, int)   ==== >
    /* auto f3 = bind(add, 1, std::placeholders::_1); */
    // int (int, int)
    // int (int, int)
    /* auto f3 = bind(add, std::placeholders::_2, std::placeholders::_1); */
    std::function<int(int, int)> f3 = bind(add, std::placeholders::_2, std::placeholders::_1);
    cout << "f3(45) = " << f3(45, 2) << endl;

    f3 = bind(multiply, std::placeholders::_1, std::placeholders::_2);
    cout << "f3(10, 20) = " << f3(10, 20) << endl;
}

typedef int (*pFunc)();

int func2()
{
    return 10;
}

int func3()
{
    return 20;
}

int func4(int x)
{
    return x;
}

void test2()
{
    pFunc f = &func2;//注册回调函数,钩子函数
    cout << "f() = " << f() << endl;//执行回调函数

    f = func3;
    cout << "f() = " << f() << endl;

    /* f = func4;//error */
}

void func5(int x1, int x2, int x3, const int &x4, int x5)
{
    cout << "x1 = " << x1 << endl
         << "x2 = " << x2 << endl
         << "x3 = " << x3 << endl
         << "x4 = " << x4 << endl
         << "x5 = " << x5 << endl;

}

void test3()
{
    //占位符本身表明的是形参的位置
    //占位符中数字参数代表函数传递进来的第几个参数
    //cref =  const reference,引用的包装器
    int number = 400;
    auto f = bind(func5, 1, std::placeholders::_2, 
                  std::placeholders::_4, std::cref(number), number);
    number = 300;
    /* f(12, 34); */
    f(12, 34, 45, 67, 89, 123, 4567);//多余的参数没有任何意义

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

