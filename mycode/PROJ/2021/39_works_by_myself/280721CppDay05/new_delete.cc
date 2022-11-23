#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class OnlyStack {
public:
    OnlyStack()
    : _pstr(nullptr){
        cout << "OnlyStack()" << endl;
    }
    OnlyStack(const char * pstr)
    : _pstr(new char[strlen(pstr) + 1]()){
        cout << "OnlyStack(const char * pstr)" << endl;
        strcpy(_pstr, pstr);
    }
    ~OnlyStack(){
        cout << "~OnlyStack()" << endl;
        if(_pstr){
            delete _pstr;
            _pstr = nullptr;
        }
    }
private:
    void* operator new(size_t nt){
        cout << "void* operator new(size_t nt)" << endl;
        void* pstr = malloc(nt);
        return pstr;
    }
    void operator delete(void *p){
        operator<<(cout, "void operator delete(void *p)");
        free(p);
    }
private: 
    char * _pstr;
};
class OnlyHeap {
public:
    OnlyHeap()
    : _pstr(nullptr){
        cout << "OnlyHeap()" << endl;
    }
    OnlyHeap(const char * pstr)
    : _pstr(new char[strlen(pstr) + 1]()){
        cout << "OnlyHeap(const char * pstr)" << endl;
        strcpy(_pstr, pstr);
    }
private:
    ~OnlyHeap(){
        cout << "~OnlyHeap()" << endl;
        if(_pstr){
            delete _pstr;
            _pstr = nullptr;
        }
    }
public:
    void destroy(){
        delete this;
    }
    void* operator new(size_t nt){
        cout << "void* operator new(size_t nt)" << endl;
        void* pstr = malloc(nt);
        return pstr;
    }
    void operator delete(void *p){
        operator<<(cout, "void operator delete(void *p)") << endl;
        free(p);
    }

private: 
    char * _pstr;

};
int main()
{
    OnlyStack os;
    OnlyStack os1("ghost");
    /* OnlyStack *os = new OnlyStack(); */

    /* OnlyHeap oh1; */
    OnlyHeap *oh2 = new OnlyHeap();
    oh2->destroy();
    return 0;
}

