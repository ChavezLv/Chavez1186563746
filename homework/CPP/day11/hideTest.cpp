#include<iostream>
using namespace std;
class Base
{
public:
	Base(int i) 
	{
		b = i;
	}

    Base():b(0){}
    virtual void Print()
    {	cout << "Base 's Print() called." << endl;}
protected:
	int b;
};
class Derived2
:public Base
{
public:
    void Print()//自动识别为虚函数
    {
        cout << "Derive2's Print() called. "<< endl;
    }
};
int main(void)
{ 
    Derived2 d2;
    d2.Print();
    d2.Base::Print();
	return 0;
}
