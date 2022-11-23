#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

class CowString {
	//设计模式之代理模式
	class CharProxy {
	public:
		CharProxy(CowString & self, size_t idx)
		: _self(self), _idx(idx)
		{}

		char & operator=(char ch);

#if 0
		operator char() {
			cout << "operator char()" << endl;
			return _self._pstr[_idx];
		}
#endif
	private:
		CowString & _self;
		size_t _idx;
	};
public:
	CowString()
	: _pstr(malloc())
	{	
		cout << "CowString()" << endl;	
		//初始化引用计数
		initRefcount();//可读性更强
	}

	CowString(const char * pstr);
	CowString(const CowString & rhs);
	CowString & operator=(const CowString & rhs);
	~CowString();

	//非const版本，不知道的操作是读操作还是写操作
	//因此让它返回一个代理类，由代理类来完成接下来的行为
	CharProxy operator[](size_t idx);
	//const版本不修改数据，返回值还是char&
	const char &  operator[](size_t idx) const 
	{
		cout << "operator[](size_t) const" << endl;
		if(idx < size()) {
			return _pstr[idx];
		} else {
			static char ch = '\0';
			return ch;
		}
	}

	const char * c_str() const {	return _pstr;	}
	int size() const {	return strlen(_pstr);}

	int use_count() const {	return *reinterpret_cast<int*>(_pstr - kRefcoutLength); }

	friend ostream & operator<<(ostream & os, const CowString & rhs);
private:
	char * malloc(const char * pstr = nullptr)
	{
		if(pstr) {
			return new char[strlen(pstr) + 1 + kRefcoutLength]() + kRefcoutLength;
		} else {
			return new char[1 + kRefcoutLength]() + kRefcoutLength;
		}
	}

	void initRefcount() 
	{	*reinterpret_cast<int *>(_pstr - kRefcoutLength) = 1;	}

	void increaseRefcount() 
	{ ++*reinterpret_cast<int *>(_pstr - kRefcoutLength);}

	void decreaseRefcount() 
	{ --*reinterpret_cast<int *>(_pstr - kRefcoutLength);}

	void release() {
		decreaseRefcount();// O(1)
		if(use_count() == 0) {
			delete [] (_pstr - kRefcoutLength);
			_pstr = nullptr;
			cout << ">> delete heap " << endl;
		}
	}

private:
	static const int kRefcoutLength = 4;
	char * _pstr;
};
 
ostream & operator<<(ostream & os, const CowString & rhs) 
{
	os << rhs._pstr;
	return os;
}

CowString::~CowString()
{
	cout << "~CowString()" << endl;
	release();
}

CowString::CowString(const char * pstr)
: _pstr(malloc(pstr))
{
	cout << "CowString(const char *)" << endl;
	strcpy(_pstr, pstr);
	initRefcount();
}

CowString::CowString(const CowString & rhs)
: _pstr(rhs._pstr) //浅拷贝  时间复杂度为O(1)
{
	increaseRefcount();// O(1)
	cout << "CowString(const CowString&)" << endl;
}
	
CowString & CowString::operator=(const CowString & rhs)
{
	cout << "CowString & operator=(const CowString&)" << endl;
	if(this != &rhs) {//考虑自复制
		release();	  //回收左操作数的空间

		_pstr = rhs._pstr;//浅拷贝  O(1)
		increaseRefcount();//引用计数加1  O(1)
	}
	return *this;
}

//问题: 下标访问运算符无法判断接下来的操作是读操作还是写操作
//解决思路：让下标运算符能够区分出读写操作

CowString::CharProxy CowString::operator[](size_t idx)
{
    cout<<"代理"<<endl;
	return CharProxy(*this, idx);
}
	
//该方法用来实现写操作
char & CowString::CharProxy::operator=(char ch)
{
	if(_idx < _self.size()) {
		//认为接下来的操作是写操作
		if(_self.use_count() > 1) {
			//原来的引用计数先减1
			_self.decreaseRefcount();
			//进行深拷贝
			char * ptmp = _self.malloc(_self._pstr);
			strcpy(ptmp, _self._pstr);
			//改变指向
			_self._pstr = ptmp;
			_self.initRefcount();
		}
		//执行写操作
		_self._pstr[_idx] = ch;
		return _self._pstr[_idx];	
	} else {
		cout << "out of range, idx invalid!" << endl;
		static char ch = '\0';
		return ch;
	}
}
int main(){
     CowString cstr("hello world");
    cstr[4]='O';
    const char ch=cstr[4];
    cout<<ch<<endl;
    return  0;
}
