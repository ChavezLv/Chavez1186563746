#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Singleton {
public:
    static Singleton* getInstance(){
        if(nullptr == _pIns){
            _pIns = new Singleton();
        }
        return _pIns;
    }
    static void rmInstance(){
        if(_pIns){
            operator<<(cout, "rmInstance():~Singleton()") << endl;
            delete _pIns;
            _pIns = nullptr;
        }
        
    }
private:
    Singleton(){
        cout << "Singleton()" << endl;
    }
    ~Singleton(){
        cout << "~Singleton()" << endl;
    }

private:
    int _sg;
    static Singleton *_pIns;
};

Singleton * Singleton::_pIns = nullptr;

int main()
{
    Singleton* sg1 = Singleton::getInstance(); 
    Singleton* sg2 = Singleton::getInstance(); 
    printf("%p\n", sg1);
    printf("%p\n", sg2);
    Singleton::rmInstance();
    Singleton::rmInstance();

    

    return 0;
}

