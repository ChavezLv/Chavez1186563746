

// 测试一：单个虚继承，不带虚函数
//	虚继承与继承的区别
//	１.　多了一个虚基指针
//	２.　虚基类子对象位于派生类存储空间的最末尾(先存不变的后存共享的)


// 测试二：单个虚继承，带虚函数
//   1.如果派生类没有自己新的虚函数，此时派生类对象不会产生虚函数指针
//   2.如果派生类拥有自己新增的虚函数，此时派生类对象就会产生自己本身的虚函数指针(指向新增的虚函数)，
//     并且该虚函数指针位于派生类对象存储空间的开始位置
//

#pragma vtordisp(off)
#include <iostream>
using std::cout;
using std::endl;

class A
{
public:
	A() : _ia(10) {}

	virtual
	void f()
	{
		cout << "A::f()" << endl;
	}
private:
	int _ia;
};

class B
: virtual public A //继承A，对A的函数有吸收的过程
{
public:
	B() : _ib(20) {}

	void fb()
	{
		cout << "A::fb()" << endl;
	}

	virtual 
	void f()//虚函数-->A也定义了所以写不写都一样
	{
		cout << "B::f()" << endl;
	}

	virtual 
	void fb2()
	{
		cout << "B::fb2()" << endl;
	}

private:
	int _ib;
};

int main(void)
{
	cout << sizeof(A) << endl;
	cout << sizeof(B) << endl;
	B b;
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
