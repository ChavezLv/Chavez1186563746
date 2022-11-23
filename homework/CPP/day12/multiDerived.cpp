#include <iostream>
using std::cout;
using std::endl;

class A{
public:
	virtual void a() {	cout << "A::a()" << endl;	}
	virtual void b() {	cout << "A::b()" << endl;	}
	virtual void c() {	cout << "A::c()" << endl;	}
};
 
class B{
public:
	virtual void a() {	cout << "B::a()" << endl;	}
	virtual void b() {	cout << "B::b()" << endl;	}
	void c() {	cout << "B::c()" << endl;	}
	void d() {	cout << "B::d()" << endl;	}
};

class C
: public A
, public B
{
public:
	virtual void a() {	cout << "C::a()" << endl;	}
    void b(){cout<<"C::b()"<<endl;}
	void c() {	cout << "C::c()" << endl;	} 
	void d() {	cout << "C::d()" << endl;	}
};

class D
: public C
{
public:
	void c() {	cout << "D::c()" << endl;	}
};

void test0() 
{
	A a;
	C c;

	cout<<"1" << endl;
	c.a();
	c.b();//成员名冲突的二义性
	c.c();
	c.d();

	
	cout <<"2" <<endl;
	A * pa = &c;
	pa->a();//C::a()
	pa->b();//A::b()
	pa->c();//C::c() //虚函数机制调用
	//pa->d();//A类中没有d方法，无法调用
	
	cout <<"3" <<endl;
	B * pb = &c;
	pb->a();//C::a()
	pb->b();//B::b()
	pb->c();//B::c()   在B中，c/d方法不是虚函数
	pb->d();//B::d()

	cout <<"4" <<endl;
	C * pc = &c;
	pc->a();
	//pc->b();//二义性
	pc->c();//?虚函数还是普通成员函数处理呢？
	pc->d();//因为C::c()已经在第一个虚函数表中了，所以当虚函数处理
	cout <<"5" <<endl;
 
	D d;
	pc = &d;
	pc->c();//D::c()  ==> 表现的就是虚函数机制
} 
 
int main(void)
{
	test0();
	return 0;
}
