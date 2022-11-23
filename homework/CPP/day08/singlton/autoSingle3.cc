#include <iostream>
using std::cout;
using std::endl;


//单例对象的自动释放方式二： atexit函数 + destroy函数
class Singleton{
public:
    static Singleton*getInstance(){
        if(nullptr==_pInstance){
            atexit(destroy);//第一次创建单例对象的时候就注册销毁函数
            _pInstance=new Singleton();
        }
            return _pInstance;
    }

    static void destroy(){
        if(_pInstance){
            delete _pInstance;
            _pInstance=nullptr;
            cout<<"the object which named _pInstance has been destroy"<<endl;
    }}
        void init(int ix,int iy){
            _ix=ix;
            _iy=iy;
            cout<<"ix="<<_ix<<endl<<"iy="<<_iy<<endl;
        }
    Singleton(const Singleton& rhs)=delete;
    Singleton& operator=(const Singleton& rhs)=delete;
private:
    Singleton(){
        cout<<"Singleton:default construct"<<endl;
    }
    ~Singleton(){
        cout<<"~Singleton()"<<endl;
    }
    static Singleton* _pInstance;
    int _ix=0;
    int _iy=0;
};
//类外操作,但是初始化操作,可以认为还是在类内部操作,所以可以访问_pInstance以及调用getInstance不用限定域
Singleton* Singleton::_pInstance=getInstance();//饿汉模式,在进入main函数之前完成初始化;避免多线程争夺而引发内存泄露
int main(){
    Singleton::getInstance()->init(1000,500);
    return 0;
}
