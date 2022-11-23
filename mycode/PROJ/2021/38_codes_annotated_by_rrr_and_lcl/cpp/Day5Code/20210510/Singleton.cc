#include <iostream>

using std::cout;
using std::endl;

//设计要求：一个类只能创建一个对象
//应用场景：对于全局唯一的对象，全局唯一的资源，应用：网络库、词典库、日志记录器 //其实cin和cout也是这样的对象
class Singleton
{
public:
    static Singleton* getInstance()//获取实例：1-有则直接返回已有指针 2-么有则新建
    {
        if(_pInstance == nullptr)//建立之前检查
        {
            _pInstance =  new Singleton();
        }
        return _pInstance;
    }

    static void destroy()//销毁函数：目前的写法必须在外部调用，然后堆空间回收
    {
        /* this->~Singleton();//error */
        if(_pInstance)
        {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }



private:
    Singleton()//私有化析构和构造函数 禁止创建
    {
        cout << "Singleton()" << endl;
    }

    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }
private:
    static Singleton* _pInstance;//数据成员：指向实例的指针
};

Singleton* Singleton::_pInstance = nullptr;//饿汉模式

/* Singleton gS1;//全局对象,error */
/* Singleton gS2;//全局对象,error */
/* Singleton gS3;//全局对象 */

int main(int argc, char **argv)
{
    /* Singleton s1;//栈对象,error */
    /* Singleton s2;//栈对象 */

    /* Singleton *ps1 = new Singleton();//堆对象,error */
    /* Singleton *ps2 = new Singleton(); */
    /* cout << "ps1 = " << ps1 << endl */
    /*      << "ps2 = " << ps2 << endl; */
    Singleton *ps1 = Singleton::getInstance();
    Singleton *ps2 = Singleton::getInstance();
    Singleton *ps3 = Singleton::getInstance();//都是同一个
    cout << "ps1 = " << ps1 << endl
         << "ps2 = " << ps2 << endl
         << "ps3 = " << ps3 << endl;

    /* delete ps1;//error, 这么写 */
    /* ps1->~Singleton();//error */
    Singleton::destroy();
    Singleton::destroy();//只有第一个生效
    Singleton::destroy();
    /* ps1->destroy(); */
    return 0;
}

