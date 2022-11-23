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


class C;
class B{
    friend class C;
private:
    int _y;
};
class C {
public:
    void p(){
        b._y;
    }
    C(int x, B& b, int y){
        _x = x;
        b._y = y;
    }

    void print(const B& b)const{
        cout << _x << " " << b._y;
    }
private:
    int _x;
    B b;
};
int main()
{
    B b;
    C c(3, b, 4);
    c.print(b);
    
    return 0;
}

