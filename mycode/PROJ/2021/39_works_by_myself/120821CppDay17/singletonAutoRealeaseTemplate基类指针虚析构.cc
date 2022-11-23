#include <iostream>
#include <pthread.h>
#include <iostream>
/* using namespace std; */
using std::cout;
using std::endl;

//内部类+静态对象

template<typename _Tp>
class Singleton {
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

public:
    /* static Singleton* getInstance(Args ... args){ */
    virtual
    void print() const = 0;
    template<typename ... Args> 
    static Singleton<_Tp> *getInstance(Args ... args){//当用基类指针指向这个new的堆空间时, 返回的类型也要变
        /* pthread_once(&_once, init(args ...)); // error*/
        if(!_pInstance){
            _pInstance = new _Tp(args ...);
            atexit(rmInstance);//自动析构除了写这里还能写哪里? 
            //虚析构函数里面是不行的,   似乎就成了循环调用了, 在自己的析构里面delete自己的成员
        }
        return _pInstance;
    }
private:
    static void  rmInstance(){
        if(_pInstance){
            delete _pInstance;
            _pInstance = nullptr;
        }
    }
    //error
    /* template<typename ... Args> */ 
    /* static void init(Args ... args){ */
    /*     if(!_pInstance){ */
    /*         _pInstance = new Singleton(args ...); */
    /*     } */
    /* } */

protected:
    /* template<typename ... Args> */  
    /* Singleton(Args ... args){ */ 
    /*     cout << "Singleton()" << endl; */ 
    /* }; //error 单例类不用创建实例了 */
    Singleton() = default;//防止实例
    Singleton(const Singleton&) = delete ;
    Singleton & operator=(const Singleton&) = delete ;

    /* virtual */
    ~Singleton(){
        cout << "~Singleton()" << endl;
        
    };
private:
    static Singleton<_Tp> * _pInstance;
    /* static _Tp * _pInstance; //改成用基类指针指向派生类对象试试, 此时不能自动析构*/
    static AutoRelease _auto;//创建私有类的全局变量 使得可以自动释放
};

template<typename _Tp>
Singleton<_Tp>* Singleton<_Tp>::_pInstance = nullptr;
/* Singleton<_Tp>* Singleton<_Tp>::_pInstance = nullptr; */

template<typename _Tp>
typename Singleton<_Tp>::AutoRelease Singleton<_Tp>::_auto;//Singleton<_Tp>::AutoRelease前面须typename
             //编译器不能识别"XXXX::XX"是个啥, 类类型还是静态成员变量? 所以要指定

/* template<typename _Tp> */
/* pthread_once_t Singleton<_Tp>::_once = PTHREAD_ONCE_INIT; */

class Point;
class Computer;

void test();
void test1();

int main()
{
    test();
    test1();
    return 0;
}

class Point//私有构造
: public Singleton<Point> //有无继承无影响
{
private:
    friend Singleton<Point>;//方便基类调用构造

    Point(double x = 0, double y = 0)
    : _x(x)
    , _y(y){
        cout << "Point(double x = 0, double y = 0)" << endl;
    }
    void print() const{
        cout << "(" << _x << "," <<  _y << ")" << endl;
    }

    ~Point(){ cout << "~Point()" << endl; }
private:
    double _x;
    double _y;
};

class Computer//私有构造
: public Singleton<Computer>
{
private:
    /* friend Singleton<Computer>;//ok */
    friend class Singleton<Computer>;
    Computer(std::string brand = "", double price = 0)
    : _brand(brand)
    , _price(price){
        cout << "Computer(std::string brand = "", double price = 0)" << endl;
        cout << _brand << _price << endl;
    }
    ~Computer(){ cout << "~Computer()" << endl; }
public:    
    void print() const{
        cout << "brand(" << _brand << "), price(" << _price << ")" << endl;
    }
private:
    std::string _brand;
    double _price;
};

void func(const Singleton<Point>* rhs){
    rhs->print();
}

void func1(const Singleton<Computer>* rhs){
    rhs->print();
}

void test(){
    /* Computer *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666); */
    Singleton<Computer> *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    //为什么此时用基类指针指向派生类对象也没由自动析构?//因为静态内部类写法有问题
    Singleton<Computer> *pc2 = Singleton<Computer>::getInstance("HuaWei", 6666);
    func1(pc1);
    func1(pc2);
    /* cout << pc1 << endl; */
    /* cout << pc2 << endl; */
}
void test1(){
    /* Point *pt3 = Singleton<Point>::getInstance(1, 2); */
    Singleton<Point> *pt3 = Singleton<Point>::getInstance(1, 2);
    Singleton<Point> *pt4 = Singleton<Point>::getInstance(1, 2);
    func(pt3);
    func(pt4);
    /* cout << pt3 << endl; */
    /* cout << pt4 << endl; */
}
