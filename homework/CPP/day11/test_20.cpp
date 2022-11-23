#include<iostream>
using namespace std;
class Base
{
public:
	Base(int i) 
	{
		b = i;
	}
    //(1)重载箭头指针运算符
    //typedef void (*func)();
    Base* operator->(){
        return this;
    }
    Base():b(0){}
    virtual void Print()
    {	cout << "Base 's Print() called." << endl;}
protected:
	int b;
};

class Derived1
:public Base
{
public:
    Derived1(int i=0)
        :Base(i){}
    virtual//(2),不加virtual也行　　　
	void Print()
	{
		cout<<"Derive1's Print() called."<<endl;
	}
};

class Derived2
:public Base
{public:
	Derived2(int i=0):Base(i){}
    //(3)　　
public:
    void Print()
    {
        cout << "Derive2's Print() called. "<< endl;
    }
};
//Base  * obj
void fun(Base& obj)
{   //Base* obj=&rOjb;
	obj->Print();//引用本质上也是一个指针
}

int main(void)
{ 
//	(5)　　　
    
    Derived1 d1;
    Derived2 d2;
    fun(d1);
    fun(d2);
	return 0;
}
