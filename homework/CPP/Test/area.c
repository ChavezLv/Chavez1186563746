#include<iostream>
using namespace std;
class Rectangle{
public:
    float high;
    float length;
    Rectangle(float h,float l){
        high=h;
        length=l;
    }
    float perimeter(){
        return 2*(high+length);
    }
    float area(){
        return high*length;
    }
};
int main(){
    
    Rectangle a(3.5,6.6);
    cout<<"perimeter of a is "<<a.perimeter()<<endl<<"area of a is "<<a.area()<<endl;
    Rectangle b(3.9,9.6);
    cout<<"perimeter of b is "<<b.perimeter()<<endl<<"area of b is "<<b.area()<<endl;

}
