#include <assert.h>
#include<iostream>
using namespace std;
template<typename S,typename ...Args>
class Singleton{
public:
    //S应该是类对象,获取对象指针;
    template<typename T,typename Y>
    static S *getInstance(T t,Y y){
        if(!_pInstance){
            _pInstance=new S(t,y);
        }
        return _pInstance;
    }
    ~Singleton(){
        if(_pInstance){
            delete[] _pInstance;
            _pInstance=nullptr;
        }
    }
private:
    static S* _pInstance;
};
template<typename S,typename ...Args>
S* Singleton<S,Args...>::_pInstance=nullptr;

class Point{
public:
    Point(int a,int b)
        :_a(a),_b(b){}
    int _a;
    int _b;
};
class Computer{
public:
    Computer(string name,int price):_name(name),_price(price){}
    string _name;
    int _price;
};

Point pt(1, 2);
Point pt2(3, 4);

Computer com("Mac", 8888);

int main(void)
{
#if 1
    Computer *pc1 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    Computer *pc2 = Singleton<Computer>::getInstance("Xiaomi", 6666);
    assert(pc1 == pc2);
#endif
    Point *pt3 = Singleton<Point>::getInstance(1, 2);
    Point *pt4 = Singleton<Point>::getInstance(1, 2);
    if(pt3 == pt4)cout<<"success"<<endl;
    else cout<<"fail"<<endl;
    //assert(p3==p4);//断言,错误终止,向stderr打印错误信息;
    
    return 0;
} 
