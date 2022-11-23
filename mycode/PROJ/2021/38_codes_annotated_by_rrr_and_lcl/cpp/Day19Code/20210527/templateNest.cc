#include <iostream>
using namespace std;

template<class T>
class Outside  //外部Outside类定义
{
public:
    template <class R>
    class Inside   //嵌套类模板定义
    {
    public:
        Inside(R x) //模板类的成员函数可以在定义时实现
        {
            r = x;
        }
        //void disp();
        void disp() 
        {
            cout << "Inside: " << r << endl;
        }
    private:
        R r;
    };

    Outside(T x) 
    : t(x) //Outside类的构造函数
    {

    }

    //void disp();
    void disp()
    {
        cout<<"Outside:";
        t.disp();
    }

private:
    Inside<T> t;
};

//disp函数如果在类外面实现的话，就需要这么嵌套的定义
//template<class T>
//template<class R>
//void Outside<T>::Inside<R>::disp()    //模板类的成员函数也可以在定义外实现
//{         
//但必须是在所有类定义的外边,不能放在Outside内Inside外去实现.
//  cout<<"Inside: "<<Outside<T>::Inside<R>::r<<endl;
//}

//template<class T>
//void Outside<T>::disp()
//{
//  cout<<"Outside:";
//  t.disp();
//}

int main()
{
    Outside<int>::Inside<double> obin(3.5); //声明Inside类对象obin
    obin.disp();

    Outside<int> obout(2); //创建Outside类对象obout
    obout.disp();

    return 0;
}
