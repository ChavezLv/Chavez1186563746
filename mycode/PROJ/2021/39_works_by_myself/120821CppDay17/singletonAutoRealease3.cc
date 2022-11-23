#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <iostream>
/* using namespace std; */
using std::cout;
using std::endl;

//内部类+静态对象
class Singleton {
public:
    static Singleton* getInstance(){
        pthread_once(&_once, init);//only once to call init,确保线程安全 
        return _pInstance;
    }

    static void init(){
        if(!_pInstance){
            _pInstance = new Singleton();
            atexit(rmInstance);
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
    };
    ~Singleton(){
        cout << "~Singleton()" << endl;
    };
private:
    static Singleton * _pInstance;
    static pthread_once_t _once;
};

Singleton* Singleton::_pInstance = nullptr;
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;


void test(){
    Singleton * pIns = Singleton::getInstance();

}
int main()
{
    test();
    return 0;
}

