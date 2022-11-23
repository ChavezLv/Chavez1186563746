#include <pthread.h>
#include <iostream>
/* using namespace std; */
using std::cout;
using std::endl;


class AutoRelease;
class Singleton {
    friend class AutoRelease;
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

class AutoRelease{
public:
    AutoRelease(){
        cout << "AutoRelease()" << endl;
    }
    ~AutoRelease(){
        cout << "~AutoRelease()" << endl;
        if(Singleton::_pInstance){
            delete Singleton::_pInstance;
            Singleton::_pInstance = nullptr;
        }
    }
};

void test(){
    Singleton * pIns = Singleton::getInstance();
    AutoRelease pr;
    /* AutoRelease *pr = new AutoRelease();//堆对象不会主动释放 */
}
int main()
{
    test();
    return 0;
}

