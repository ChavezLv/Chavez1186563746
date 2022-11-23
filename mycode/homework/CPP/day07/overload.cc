#include<iostream>
using namespace std;
class Base{
public:
    Base(int val){
        i=val;

    }
    int operator+(Base& rhs){
        return rhs.i-this->i;
    }
private:
    int i;
};
int main(){
    int i=4;
    int j=1;
    Base x(i);
    Base y(j);
    cout<<(x+y==j-i)<<endl;
}
