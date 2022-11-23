#include<iostream>
using namespace std;
class Cylinder{
public:
    Cylinder(double r,double h)
    :_h(h)
    ,_r(r)
    {
        cout<<"the bottom radius of Cylinder is:"<<_r\
            <<",and the high of Cylinder is:"<<_h<<endl;
    }
    double getVolume(){
        return _pi*_r*_r*_h;
    }
    void show(){
        cout<<"the volume of Cylinder is:"<<getVolume()<<endl;
    }
private:
    double _h;
    double _r;
    double _pi=3.141592653589;
};

int main(){
    double r=6.62607015;
    double h=8.314472;
    Cylinder B(r,h);
    B.show();
    return 0;
}
