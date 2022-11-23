#include<iostream>
#include<functional>
using std::cout;
using std::endl;
using std::function;

typedef void(*Function)(int);//函数返回值和参数列表确定函数类型,此时可以把函数指针理解为函数类型;
                             //定义了一种返回值为void,参数为int的新类型,并且该类型为指向函数的指针;
void test0(int x){
    static int cnt=0;//局部静态变量,离开函数后无法访问,一直驻留在全局数据区至程序结束;
    cout<<"test0()"<<endl;
    ++cnt;
}

void test(int x){
    cout<<"test1"<<endl;
}

class FunctionObject{//函数调用符重载,函数对象
public:
    int operator()(){
        cout<<"FunctionObject operator()()"<<endl;
        return 1;
    }
    int operator()(int x, int y) {
		cout << "operator()(int,int)" << endl;
		++_cnt;
		return x + y;
	}

	int operator()(int x, int y, int z) {
		return x + y + z;
	}
    int _cnt=0;
};
//
struct Foo{
	void print()
	{	cout << "Foo::print()" << endl;}

	void display()
	{	cout << "Foo::display()" << endl;	}
};
typedef void (Foo::*MemFunction)();
void test1(){
    &test0;//获取成员函数地址,值和test(指针)是一样的,但是类型不一样
    //通过函数指针调用
    MemFunction f0=&Foo::print;
    //通过对象调用
    Foo foo;//通过对象调用
	(foo.*f0)();//成员函数指针的调用

    //通过指针调用
	Foo * fp = nullptr;
	f0 = &Foo::display;
	(fp->*f0)();
}



void print()
{	cout << "void print()" << endl;	}

void display()
{	cout << "void display()" << endl;	}

typedef void(*FuncPointer)();


struct Test {
	//print方法有一个隐含的this指针
	void print() const {
		cout << "Test::print()" << endl;
	}
};

#if 0
//针对于成员函数的， std::function的实现原理
class function {
public:
	void operator()()
	{
		(fp->*f)();
	}

	//void(Test::*MemberFunction)()
	//保存成员函数的入口地址,通过bind传递过来
	MemberFunction f = &Test::print;
	//保存对象的地址,通过bind传递过来
	Test * fp = &test;
};

#endif
//
void test3()
{
	//可以把std::function看成是一个函数的容器
	//尖括号中的是函数类型:  返回值+ 形参列表
	//
	//function统筹了所有的函数形式
	function<void()> f = print;
	f();

	f = display;
	f();

	//C语言函数指针的用法
	FuncPointer  f0 = print;

	cout << "\nfunction容纳的是函数指针:" << endl;
	f = f0;
	f();

	cout << "\nfunction容纳的是成员函数:" << endl;
	//当面对的是成员函数时，必须要提前绑定参数,
	//因为成员函数必须要通过对象进行调用
	Test test;
	//std::bind返回的就是一个std::funciton对象
	//
	//funciton + bind的组合会大量使用
	f = std::bind(&Test::print, &test);
	f();//f.operator()();
	//f() 最终等价于 test.print();
}

struct Bar {
	void test0() {	cout << "Bar::test0()" << endl;	}
	void test1(int x) {
		cout << "Bar::test1(), x: " << x << endl;
	}

	void test2() {
		cout << "Bar::test2(), _data:" << this->_data << endl;
	}

	friend class Test;
	int _data = 10;
};

void test2() {
	Bar * fp = nullptr;
	fp->test0();   //Bar::test0(nullptr);
	fp->test1(1);  //Bar::test1(nullptr, 1);
	fp->test2();   //Bar::test2(nullptr);
}

int main(){
    Function f=test0;//此时的test0作为值传递,编译器会将函数名替换为指针,即将函数首地址付给指针f;
    f(1);

    FunctionObject fo;
    cout << fo() << endl;
	cout << fo.operator()() << endl;
	cout << fo(1, 2) << endl;//这是简写形式
	cout << fo.operator()(1, 2) << endl;
}
