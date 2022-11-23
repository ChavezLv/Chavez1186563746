#include <iostream>

using std::cout;
using std::endl;

//2、内部类 + 静态数据成员//静态数据成员也会随着类的消亡而消亡
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(nullptr == _pInstance)
        {
            _pInstance = new Singleton();
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
    }

    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }
private:
    class AutoRelease
    {
    public:
        AutoRelease()
        {
            cout << "AutoRelease()" << endl;
        }
    
        ~AutoRelease()
        {
            cout << "~AutoRelease()" << endl;
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
private:
    static Singleton *_pInstance;
    static AutoRelease _auto;//静态数据成员生存期一致，而且对于用户而言封装性更好
};

/* Singleton *Singleton::_pInstance = nullptr; */
Singleton *Singleton::_pInstance = Singleton::getInstance();
Singleton::AutoRelease  Singleton::_auto;

int main(int argc, char **argv)
{
    Singleton *ps1 = Singleton::getInstance();
    Singleton *ps2 = Singleton::getInstance();
    /* Singleton::AutoRelease ar;//栈对象//error */

    /* Singleton::destroy(); */
    return 0;
}

