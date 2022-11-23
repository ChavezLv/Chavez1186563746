 ///
 /// @file    tls.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-22 11:16:25
 ///
 
#include <errno.h>

#include <iostream>
using std::cout;
using std::endl;
// ----------lcl----新概念: 线程栈--------------------------------
//每一个errno的地址都是不同的, 说明并不是同一个errno
//errno是一个线程局部存储的变量
//如何理解 "errno是一个线程局部存储的变量"?
//定义一个__thread int tNumber, 打印地址也是不一样的
//但是普通的全局变量
__thread int tNumber = 0;//一旦定义了该tNumber, 每起一个线程就会有一个
/* __thread std::string str("wd"); */ // error 错误的
int gNumber = 1;//全局变量


// __thread并不是适合所有的类型, POD类型就是c的写法, c++的不可以
/*   POD类型在源代码兼容于ANSI C时非常重要

比如: __thread string* t_object_2 = new std::string (); // 错误，初始
化必须用编译期常量, new string()是运行时的
__thread string* t_object_3 = nullptr;// 正确，空指针就可以, 需要手工初始化并
销毁对象

*/
// 所以刚才的_name这个问题就可以解决掉了!
/*
回到Thread.hpp  


*/
void * threadFunc1(void * arg)
{

	int thread1 = 0;// 线程内部自定义一个int, 其地址也是在线程栈内, 所以地址和tNumber比较相近
	tNumber = 1;// 这样每个线程都会有
	printf("&errno : %p, &tNumber:%p, tNumber:%d\n", &errno, &tNumber, tNumber);
	printf("&gNumber:%p, &thread1:%p\n", &gNumber, &thread1);
}

void * threadFunc2(void * arg)
{
	tNumber = 2;
	printf("&errno : %p, &tNumber:%p, tNumber:%d\n", &errno, &tNumber, tNumber);
	printf("&gNumber:%p\n", &gNumber);

}

 
void test0() 
{
	tNumber = 3;
	printf("&errno : %p, &tNumber:%p, tNumber:%d\n", &errno, &tNumber, tNumber);
	printf("&gNumber:%p\n", &gNumber);

	pthread_t pthid1, pthid2;
	pthread_create(&pthid1, nullptr, threadFunc1, nullptr);
	pthread_create(&pthid2, nullptr, threadFunc2, nullptr);

	pthread_join(pthid1, nullptr);
	pthread_join(pthid2, nullptr);
} 
 
int main(void)
{
	test0();
	return 0;
}
