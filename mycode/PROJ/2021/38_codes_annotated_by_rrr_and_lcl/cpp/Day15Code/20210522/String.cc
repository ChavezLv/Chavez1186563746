#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class String //类string
{
public:
    String()
    : _pstr(nullptr)//默认构造函数：先生成空指针
    {
        cout << "String()" << endl;
    }

    String(const char *pstr)//构造函数:c_str->string
    : _pstr(new char[strlen(pstr) + 1]())
    {
        cout << "String(const char *)" << endl;
        strcpy(_pstr, pstr);
    }

    String(const String &rhs)
    : _pstr(new char[strlen(rhs._pstr) + 1]())
    {
        cout << "String(const String &)" << endl;//经典左值拷贝
        strcpy(_pstr, rhs._pstr);//多出一个拷贝
    }

    String &operator=(const String &rhs)//经典左值拷贝赋值
    {
        cout << "String &operator=(const String &)" << endl;
        if(this != &rhs)//对比下面其实就会发现
        {
            delete [] _pstr;
            _pstr = nullptr;

            _pstr = new char[strlen(rhs._pstr) + 1]();//这步是比移动语义多出来的
            strcpy(_pstr, rhs._pstr);//这这步也是比移动语义多出来的
        }

        return *this;
    }

    //带有移动语义的函数, 默认情况下, 编译器不会自动生成的, 必须要手写

    //String s2 = String("world")
    //移动构造函数(移动语义): 右值堆空间转移给了左值_pstr
    // 和拷贝构造函数, 都能识别右值, 但是移动构造函数的优先值更高一些. 
    
    // 用的是浅拷贝, 有必要加上const吗? 没有必要, 
    //本来不加const也可以绑定到右值上, 加上之后不是更可以吗
    //所以一般没有必要的 
    String(String &&rhs)//构造函数特征：1-与类同名 2-参数是一个引用（前面藏了一个this）//左值引用or右值引用
    : _pstr(rhs._pstr) //直接用右操作数的内部指针初始化左操作数
    {//有一个很神奇的事情在此被验证了：右值并不是【内部没有占内存】，只是由于各种各样的原因【取地址符号拿不到】而已
        cout << "String(String &&)" << endl;
        rhs._pstr = nullptr;//右操作数置空，之后拿不到, 之后临时对象就销毁不了了
    }

    //总结: 带有复制控制语义的函数 优先级 比移动语义的函数低
    //移动赋值函数(移动语义): 优先级更高, 没有的话就会调用String &operator=(const String &rhs)
    //s3 = String("wangdao")
    //s4 = std::move(s4)
    // s5 = std::move(s4)// s4的值被掏走了, 以后就不想用它了
    String &operator=(String &&rhs) //检测到【等号右边是一个右值】
    {
        cout << "String &operator=(String &&)" << endl;
        if(this != &rhs)//1、自移动--->此时是唯一的【利用std::move】居然实现了【等号左边和右边是同一个右值】
        {//一旦满足这个【赋值对象左右可相等】，就需要处理【自赋值】的问题
            //进来则【非自赋值】

            delete [] _pstr;//释放原有左值
            _pstr = nullptr;

            _pstr = rhs._pstr;//直接把现在的【临时变量右值】的指针/空间给偷出来塞到左操作数里面
            //this->_pstr = rhs._pstr
            rhs._pstr = nullptr;//把右值指针置空，待会儿临时变量的释放就放不到什么东西
        }

        return *this;
    }

    ~String()
    {
        cout << "~String()" << endl;
        if(_pstr)//析构函数，记得先进行空指针检查
        {
            delete [] _pstr;
            _pstr = nullptr;//delete之后，记得置空指针
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs);
private:
    char *_pstr; //唯一数据对象【指向堆的指针】
};

std::ostream &operator<<(std::ostream &os, const String &rhs)
{
    if(rhs._pstr)
    {
        os << rhs._pstr;
    }

    return os;
}

void test()
{
    String s1("hello");
    cout << "s1 = " << s1 << endl;

    cout << endl;
    String s2 = s1;
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;

    cout << endl;
    String s3 = "world";//String("world");
    cout << "s3 = " << s3 << endl;

    /* &"world";//左值，位于文件常量区 */
    /* &String("world");//error,右值 */

    cout << endl;
    s3 = String("wangdao");
    cout << "s3 = " << s3 << endl;

    cout << endl << endl;
    String("world") = String("world");

    cout << endl << "s2 = std::move(s2)" << endl;
    s2 = std::move(s2);//std::move(作用: 将左值转化为右值, s2会被掏空)其实上什么都没有移动走，只是对
                       //s2做了一个强制转换，底层实现: static_cast<T &&>(lvalue)
                       //表名以后不想使用这个左值了, 
    cout << "s2 = " << s2 << endl; // 如果移动语义没有考虑自复制 , 也不行, 必须考虑std::move
    cout << "111" << endl;

    s3 = std::move(s2);//表明以后不想再去用这个s2了
    cout << "s3 = " << s3 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "222" << endl;

    s2 = "welcome";//还想用的话, 就重新赋值
    cout << "s2 = " << s2 << endl;
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

