#include<iostream>
using namespace std;
 class Singleton{
public:
    static Singleton* getInstance(){
    if(instance==nullptr){
            instance=new Singleton();
        }
        return instance;
    }
    void init(int i){
        this->id=i;
    }//初始化成员

    void destroy(){
        delete instance;
        this->~Singleton();
    }
    //Singleton()=delete;
    Singleton(const Singleton & rhs)=delete;
    Singleton & operator=(const Singleton &)=delete;
private:
    Singleton();
    ~Singleton();
    int id;
    static Singleton* instance;
};
Singleton *Singleton::instance=nullptr;
