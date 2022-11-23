#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

//3、atexit + 饿汉模式//成对使用的【配对弹出函数】

class Singleton
{
public:
    static Singleton *getInstance()
    {
        //在多线程环境下不安全
        if(nullptr == _pInstance)
        {
            _pInstance = new Singleton();
            atexit(destroy);
        }

        return _pInstance;
    }

    static void destroy()
    {
        if(_pInstance)
        {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }
private:
    Singleton()
    {
        cout << "Singleton()" << endl;
        /* atexit(destroy); */
    }

    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }
private:
    static Singleton *_pInstance;
};

/* Singleton *Singleton::_pInstance = nullptr;//饱汉（懒汉）模式 */
Singleton *Singleton::_pInstance = getInstance();//饿汉模式

int main(int argc, char **argv)
{
    Singleton *ps1 = Singleton::getInstance();
    Singleton *ps2 = Singleton::getInstance();
    /* Singleton::AutoRelease ar;//栈对象//error */

    /* Singleton::destroy(); */
    return 0;
}

