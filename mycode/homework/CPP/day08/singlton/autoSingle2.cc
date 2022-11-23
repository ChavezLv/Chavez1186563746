#include <iostream>
using std::cout;
using std::endl;


//单例对象的自动释放方式一： 嵌套类 + 静态对象 
class Singleton
{
	//该类专为Singleton服务
	class AutoRelease {
	public:
		//内部类可以直接获取外部类的静态数据成员
		//非静态数据成员无法直接访问
		AutoRelease()
		{	cout << "AutoRelease()" << endl;}

		~AutoRelease() {//直接操作_pInstance
			cout << "~AutoRelease()" << endl;
			if(_pInstance) {
				delete _pInstance;
				_pInstance = nullptr;
			}
		}
	};
public:
	static Singleton * getInstance()
	{
		if(nullptr == _pInstance) {
			_pInstance  = new Singleton();
		}
		return _pInstance;
	}

	void init(int ix, int iy)
	{	
		_ix = ix;
		_iy = iy;
	}

	void print() const 
	{
		cout << "(" << _ix
			 << "," << _iy
			 << ")" << endl;
	}

	Singleton(const Singleton & rhs) = delete;
	Singleton & operator=(const Singleton &) = delete;

private:
	Singleton() {	cout << "Singleton()" << endl;}

	Singleton(int ix, int iy)
	: _ix(ix)
	, _iy(iy)
	{	cout << "Singleton(int,int)" << endl;	}

	~Singleton() {	cout << "~Singleton()" << endl;	}


	static Singleton * _pInstance;
	//非静态的数据成员就是单例对象的数据成员
	//AutoRelease _ar;//成为单例对象的一部分,无法解决问题
	static AutoRelease _ar;

	int _ix = 0;
	int _iy = 0;
};

//在类之外初始化静态数据成员
Singleton * Singleton::_pInstance = nullptr;
//当main函数结束后该对象销毁时,自动调用析构函数区释放sigleton的堆空间;
Singleton::AutoRelease Singleton::_ar;
 

void test0() 
{
	//不希望单独创建一个AutoRelease对象
	//AutoRelease ar(Singleton::getInstance());
	//这是更常用的调用
	Singleton::getInstance()->init(10, 12);
	Singleton::getInstance()->print();
 
	//Singleton::destroy();
} 
 
int main(void)
{
	test0();
	return 0;
}
