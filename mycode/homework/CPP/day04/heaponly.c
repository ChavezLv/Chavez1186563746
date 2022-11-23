#include<string.h>
#include<iostream>
using namespace std;

class OnlyH{
public:
public:
    OnlyH(const char* n)
        :_name(new char[strlen(n)+1]()){
            cout<<"use OnlyH constructor"<<endl;
        }
    void * operator new(size_t sz);
    void   operator delete(void * p);
    void release(){
        delete this;
    }
private:
    char* _name;
    ~OnlyH(){
        if(_name){
            delete []_name;
            _name=nullptr;
        }
        cout<<"~OnlyH"<<endl;
    }
};
