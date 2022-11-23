#include <pthread.h>
#include <iostream>
using std::cout;
using std::endl;


//平台相关的实现, 只能在Linux平台之下实现
//单例对象的自动释放方式三： atexit函数 + pthread_once函数
class Singleton
{
public:
	//在多线程环境下，getInstance函数无法保证线程安全的
	static Singleton * getInstance()
	{
		pthread_once(&_once, init);//保证init函数在本进程中只执行一次;
		return _pInstance;
	}

	static void init() 
	{
		_pInstance = new Singleton();
		atexit(destroy);
	}

	static void destroy()
	{	
		if(_pInstance) {
			//在类内部运行delete表达式，就
			//可以直接调用析构函数了
			delete _pInstance;
			_pInstance = nullptr;//SAFE DELETE
		}
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


private:
	static Singleton * _pInstance;
	static pthread_once_t _once;//定义_once
	int _ix = 0;
	int _iy = 0;
};

//懒加载: 不到需要使用该对象时，就不创建
Singleton * Singleton::_pInstance = nullptr;//懒汉(饱汉)模式
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;
 

void test0() 
{
	Singleton::getInstance()->init(10, 12);
	Singleton::getInstance()->print();
} 
 
int main(void)
{
	test0();
	return 0;
}
