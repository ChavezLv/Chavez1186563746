
#include <iostream>
using std::cout;
using std::endl;


class Singleton{
public:
	static Singleton * getInstance()
	{
		if(nullptr == _pInstance) {
			_pInstance  = new Singleton();//调用了默认构造函数;
		}
		return _pInstance;
	}
    void init(int x,int y){
        _ix=x;
        _iy=y;
        cout<<"x="<<x<<endl<<"y="<<y<<endl;
    }
    Singleton(const Singleton& rhs)=delete;
    Singleton &operator=(const Singleton &rhs)=delete;
    friend class AutoRelease;
private:
    Singleton(){cout<<"singleton: default constructor"<<endl;}
    ~Singleton(){cout<<"Singleton:default desroy"<<endl;};
    static Singleton* _pInstance;
    int _ix=0;
    int _iy=0;
};

class AutoRelease{
public:
    AutoRelease(Singleton* p)
    :_p(p)
    {
        cout<<"AutoRelease()"<<endl;
    }
    ~AutoRelease(){
        cout<<"~AutoRelease()"<<endl;
        if(_p){
            delete _p;//调用了Singleton的析构函数,浅拷贝;所以要成为singleton的友元类;
            //因为是浅拷贝,所以释放了pInstance指向的堆空间,因此singleton析构函数被调用
            _p=nullptr;
        }
    }
private:
    Singleton* _p;
};
Singleton* Singleton::_pInstance=nullptr;
int main(){
    //Singleton::getInstance()->init(10,12);
    AutoRelease ar(Singleton::getInstance());
    
    return 0;
}
