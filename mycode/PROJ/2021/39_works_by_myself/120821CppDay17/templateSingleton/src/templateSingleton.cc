#include "../include/templateSingleton.hh"

//对比
class Point;//继承类
class Computer;//普通类

void test();//Computer
void test1();//Point

int main()
{
    test();
    test1();
    return 0;

}

class Point//私有构造
: public Singleton<Point> //有继承
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

    ~Point(){ cout << "~Point()" << endl;  }
private:
    double _x;
    double _y;

};

class Computer//私有构造
/* : public Singleton<Computer> */
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
    ~Computer(){ cout << "~Computer()" << endl;  }
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

void test1(){
    Singleton<Point> *pt3 = Singleton<Point>::getInstance(1, 2);//向上转型
    Singleton<Point> *pt4 = Singleton<Point>::getInstance(1, 2);//向上转型
    Point *pt5 = Singleton<Point>::getInstance(1, 2); //ok
    Point *pt6 = Singleton<Point>::getInstance(1, 2); //ok
    func(pt5);//
    func(pt6);
    cout << pt3 << endl;
    cout << pt4 << endl;

}

void func1(const Singleton<Computer>* rhs){
    rhs->print();

}

//非继承类
void test(){
    Computer *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    Computer *pc2= Singleton<Computer>::getInstance("Xiaomi", 6666);
    pc1->print();
    pc2->print();
    cout << pc1 << endl;
    cout << pc2 << endl;

    /* Singleton<Computer> *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666); */
    /* Singleton<Computer> *pc2 = Singleton<Computer>::getInstance("HuaWei", 6666); */
    /* func1(pc1); */
    /* func1(pc2); */

}

