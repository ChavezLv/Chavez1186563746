#include <iostream>
//course2 
// 笔记by lcl:\
1)区别:\
    template <typename... Args>//模板参数包\
    void print(Args... args)//函数参数包\
\
\
都记到代码里面了\
\
\
//接下来是下一天的代码class Template



using std::cout;
using std::endl;

//...位于参数包左边的时候称为打包, 右边成为解包
//template <typename T1, typename T2, typename T3, ...>
//void display(T1 t1, T2 t2, T3 t3, ...)

template <typename... Args>//模板参数包
void print(Args... args)//函数参数包
//void print(T1 t1, T2 t2, T3 t3....)
{
    cout << "sizeof...(Agrs) = " << sizeof...(Args) << endl;
    cout << "sizeof...(args) = " << sizeof...(args) << endl;
    /* cout << "...args = " << ...agrs << endl; */
}

void test()
{
    //print();
    print();

    //编译时候的推导过程:
    //template <typename T>
    //void print(T t);
    print(1);

    //print(int a, string b)
    print(1, "a");

    //编译时候的推导过程:
    //template <typename T1, typename T2, typename T3>
    //void print(T1 t1, T2 t2, T3 t3) 
    //推导为
    //print(int a, string b, bool c)
    print(1, "hello", true);
}
#if 0
template <>
void print()
{

}

template <typename T>
void print(T t1)
{

}

template <typename T1, typename T2>
void print(T1 t1, T2 t2)
{

}
#endif

void display()
{
    cout << endl;
}
template <typename T, typename... Args>
void display(T t1, Args... args)//T t1表示至少传一个参数
{
    cout << t1 << "  ";
    display(args...);//...位于函数参数的后面的时候，称为解包
    //这时候递归调用, 此时的display()这个递归出口, 必须要写出来, 否则没有实现就会报错
    
    //为什么要写三个点? 
    // 语法规则, 三个代表多, 设计时候是这么设计
}


void test2()
{
    display();//空的, 打印一个换行, 只调用无参构造

    display(1);
    //  cout << 1 << " ";
    //display();
    //cout << endl;

    display(1, "hello");
    //cout << 1 << " ";
    //display("hello");
    //   cout << "hello" << "  ";
    //   display();
    //
    
    display(1, "hello", true, 3.3);
    //cout << 1 << " ";
    //display("hello", true, 3.3);
    //   cout << "hello" << " ";
    //  display(true, 3.3);
    //     cout << 1 << "  ";
    //     display(3.3);
    //        cout << 3.3 << " ";
    //        display();
    //          cout <<endl;
}

int sum()
{
    return 0;
}

template <typename T1, typename ... Args>
int sum(T1 t1, Args... args)
{
    return t1 + sum(args...);//一直递归调用, 最后是空, \
                                    也要写出来递归调用出口int sum(){}
}
void test3()
{
    cout << "sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) = "
         << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << endl;
}
int main(int argc, char **argv)
{
    test3();
    return 0;
}

