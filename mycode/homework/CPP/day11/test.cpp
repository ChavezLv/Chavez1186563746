#include<iostream>
using namespace std;
void test0();
void test1();
void test2();
void test3();
void test4();
int main(){
    //test0();
    test1();
    return 0;
}
void test1(){
    class A
{ 
public:
    virtual
    void func(int val = 1)
    {
        cout << "A->" << val << endl;
    }
    virtual void test()
    {
        func();
    }
private:
    long _a;
};

class B
: public A
{ 
public:
    virtual
    void func(int val = 10)
    {
        cout << "B->" << val << endl;
    }
private:
    long _b;
};

class C
: public B
{
    virtual
    void func(int val = 10)
    {
        cout << "C->" << val << endl;
    }   
    
};

    B b;
    A *p1 = (A*)&b;
    B *p2 = &b;
    p1->func();
    p2->func();
   
}


 class A
{
public:
     A() 
	{ 
		cout << "A's cons." << endl; 
	}
	

	virtual 
	~A() 
	{ 
		cout << "A's des." << endl; 
	}
	
	virtual 
	void f() 
	{ 
		cout<<"A's f()."<<endl; 
	}
	
	void g() 
	{ 
		f();
	}

};

class B 
: public A
{
public:
    B() 
	{ 
		f(); 
		cout << "B's cons." << endl; 
	}
	
	~B() 
	{ 
		cout << "B's des." << endl; 
	}

};

class C 
: public B
{
public:
	C() 
	{ 
		cout<<"C's cons."<<endl; 
	}

	~C()
	{ 
		cout<<"C's des."<<endl;
	}
	
	void f() 
	{ 
		cout<<"C's f()."<<endl; 
	}

};
void test0(){
	A *pa=new C();
    pa->g();

    delete pa;
}

