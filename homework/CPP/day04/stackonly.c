#include<string.h>
#include<iostream>
using namespace std;

class OnlyS{
public:
    OnlyS(const char* n)
        :_name(new char[strlen(n)+1]()){
            cout<<"use OnlyS constructor"<<endl;
        }
    ~OnlyS(){
        if(_name){
            delete []_name;
            _name=nullptr;
        }
        cout<<"~Onlys"<<endl;
    }
private:
    void * operator new(size_t sz);
    void  operator delete(void * p);
private:
    char* _name;
    
};
