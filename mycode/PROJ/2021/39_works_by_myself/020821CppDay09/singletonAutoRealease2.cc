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
        pthread_once(&_once, init);
        return _pInstance;
    }

    static void init(){
        if(!_pInstance){
            _pInstance = new Singleton();
        }
    }
private:
    //嵌套类一般私有
    class AutoRelease{
    public:
        AutoRelease(){
            cout << "AutoRelease()" << endl;
        }
        ~AutoRelease(){
            cout << "~AutoRelease()" << endl;
            if(_pInstance){
                delete _pInstance;
                _pInstance = nullptr;
            }
        }

    };

private:
    Singleton(){
        cout << "Singleton()" << endl;
    };
    ~Singleton(){
        cout << "~Singleton()" << endl;
    };
private:
    static Singleton * _pInstance;
    static AutoRelease _auto;
    static pthread_once_t _once;
};

Singleton* Singleton::_pInstance = nullptr;
Singleton::AutoRelease Singleton::_auto;
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;


void test(){
    Singleton * pIns = Singleton::getInstance();

}
int main()
{
    test();
    return 0;
}

