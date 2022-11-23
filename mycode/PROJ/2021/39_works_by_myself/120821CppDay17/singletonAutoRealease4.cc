#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <iostream>
/* using namespace std; */
using std::cout;
using std::endl;

//pthread_once + atexit
class Singleton {
public:
    static Singleton* getInstance(){
        pthread_once(&_once, init);//only once to call init,确保线程安全 
        return _pInstance;
    }

    static void init(){
        if(!_pInstance){
            _pInstance = new Singleton();
            /* atexit(rmInstance); */
        }
    }

    static void  rmInstance(){
        if(_pInstance){
            delete _pInstance;
            _pInstance = nullptr;
        }
    }

private:
    Singleton(){
        cout << "Singleton()" << endl;
        atexit(rmInstance);//如果是饿汉模式,则在此注册
    };
    ~Singleton(){
        cout << "~Singleton()" << endl;
    };
private:
    static Singleton * _pInstance;
    static pthread_once_t _once;
};

/* Singleton* Singleton::_pInstance = nullptr; */
Singleton* Singleton::_pInstance = new Singleton();
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;


void test(){
    Singleton * pIns = Singleton::getInstance();

}
int main()
{
    test();
    return 0;
}

