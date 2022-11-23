#include"cowString.hpp"
using namespace cowString;
void test0(){
    Char ch1('w');
    Char ch2;
    ch2='c';
    cout<<ch2<<endl;
    Char ch3='q';
    cout<<ch1<<ch2<<ch3<<endl;
}
#if 0
void test1(){
    Char *CH="wangdao"
    CowString cstr(CH);
    CowString cstr1="nice";
    CowString cstr2=cstr1;
    CowString cstr3;
    cstr3=cstr1;
    CowString(cstr1);
}
#endif
#if 1
void test2()
{
	CowString str1;
	CowString str2 = str1;
	cout << "str1:" << str1 << endl;
	cout << "str2:" << str2 << endl;
	cout << "str1.use_count:" << str1.getReferenceCount() << endl;
	cout << "str2.use_count:" << str2.getReferenceCount() << endl;
	cout << endl;

	CowString str3 = "hello,world";
	cout << "str3:" << str3 << endl;
	CowString str4 = str3;
	cout << "执行拷贝构造:" << endl;
	cout << "str4:" << str4 << endl;
	cout << "str3.use_count:" << str3.getReferenceCount() << endl;
	cout << "str4.use_count:" << str4.getReferenceCount() << endl;
	printf("str3' address: %p\n", str3.c_str());
	printf("str4' address: %p\n", str4.c_str());
	cout << endl;
#endif
#if 1
	cout << "执行赋值操作:" << endl;
	str2 = str3;
	cout << "str1:" << str1 << endl;
	cout << "str2:" << str2 << endl;
	cout << "str3:" << str3 << endl;
	cout << "str4:" << str4 << endl;
	cout << "str1.use_count:" << str1.getReferenceCount() << endl;
	cout << "str2.use_count:" << str2.getReferenceCount() << endl;
	cout << "str3.use_count:" << str3.getReferenceCount() << endl;
	cout << "str4.use_count:" << str4.getReferenceCount() << endl;
	printf("str2' address: %p\n", str2.c_str());
	printf("str3' address: %p\n", str3.c_str());
	printf("str4' address: %p\n", str4.c_str());
	cout << endl;

	cout << "执行修改操作:" << endl;
	str2[0] = 'X';
	//从解决思路来看，
	//必须要保证str2[0]返回的是一个自定义类类型, 而不是char&类型

	cout << "str2:" << str2 << endl;
	cout << "str3:" << str3 << endl;
	cout << "str4:" << str4 << endl;
	cout << "str1.use_count:" << str1.getReferenceCount() << endl;
	cout << "str2.use_count:" << str2.getReferenceCount() << endl;
	cout << "str3.use_count:" << str3.getReferenceCount() << endl;
	cout << "str4.use_count:" << str4.getReferenceCount() << endl;
	printf("str2' address: %p\n", str2.c_str());
	printf("str3' address: %p\n", str3.c_str());
	printf("str4' address: %p\n", str4.c_str());
	cout << endl;

	cout << "执行读操作:" << endl;
	//str3对象不是const对象，无法调用const版本的下标访问运算符
	cout << "str3[0]:" << str3[0] << endl;
	cout << "str3.use_count:" << str3.getReferenceCount() << endl;
	cout << "str4.use_count:" << str4.getReferenceCount() << endl;
	printf("str3' address: %p\n", str3.c_str());
	printf("str4' address: %p\n", str4.c_str());
	cout << endl;
}
#endif
int main (){
//    test1();
    test2();
    return 0;
}
