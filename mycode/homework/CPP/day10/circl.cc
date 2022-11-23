#include<iostream>
using namespace std;
class Circle{
public:
    Circle()
    :_r(0)
    {
        cout<<"default radius=0"<<endl;
    }
    Circle(double r){
        _r=r;
    }
    double getArea(){
        return _pi*_r*_r;
    }
    double getPerimeter(){
        return 2*_pi*_r;
    }
    void show(){
        cout<<"The circle radius is:"<<_r<<endl\
            <<"Area of the circle is:"<<getArea()<<endl\
            <<"Perimeter of the circle is:"<<getPerimeter()<<endl;
    }
private:
    double _r;
    double _pi=3.141592653589;
};

int main(){
    Circle A;
    Circle B(8.3144);
    A.show();
    B.show();
    return 0;
}
