#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Figure {
public:
    Figure() : _name(0) {  }
    Figure(string name) : _name(name){  }
    virtual double getArea() const = 0;
    virtual string getName() const = 0;
    virtual void show() const = 0;
protected:
    string _name;

};
class Circle 
: public Figure{
private:
#define PI 3.14159265
    double _r;
    
public:
    Circle() :  _r(0){  }
    Circle(double r, string name = 0) : Figure(name), _r(r){  }
    
    //get _r
    double getRadius() const{
        return _r;
    }

    //get perimeter
    double getPerimeter() const{
        return _r * PI * 2;
    }

    double getArea() const{
        return pow(_r, 2) * PI;
    }

    string getName() const{
        return _name;
    }

    void show() const{
        cout << this << ": " << "r(" << _r << "), perimeter(" << \
                    getPerimeter() << "), area(" << getArea() << ")" << endl;
    }

};

void func(const Figure &figure){
    cout << figure.getArea() << endl;
    cout << figure.getName() << endl;
    figure.show();
}
int main()
{
    Circle circle(5, "circle1");
    func(circle);
    return 0;
}

