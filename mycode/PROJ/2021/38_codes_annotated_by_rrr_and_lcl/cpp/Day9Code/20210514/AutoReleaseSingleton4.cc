#include <pthread.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

//4、pthread_once + atexit(平台相关)

class Singleton
{
public:
    static Singleton *getInstance()
    {
        pthread_once(&_once, init);

        return _pInstance;
    }

    static void init()
    {
        _pInstance = new Singleton();
        atexit(destroy);
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
    static pthread_once_t _once;
};

Singleton *Singleton::_pInstance = nullptr;//饱汉（懒汉）模式
/* Singleton *Singleton::_pInstance = getInstance();//饿汉模式 */
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;

int main(int argc, char **argv)
{
    Singleton *ps1 = Singleton::getInstance();
    Singleton *ps2 = Singleton::getInstance();
    /* Singleton::AutoRelease ar;//栈对象//error */

    /* Singleton::destroy(); */
    return 0;
}

