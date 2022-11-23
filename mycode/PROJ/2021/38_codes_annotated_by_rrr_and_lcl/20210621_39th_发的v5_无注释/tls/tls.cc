 ///
 /// @file    tls.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-21 15:57:38
 ///

#include <stdio.h>
#include <errno.h>


#include <iostream>
using std::cout;
using std::endl;
 
void test0() 
{
 
} 

int gNumber = 100;

__thread int tNumber = 0;

void * threadFunc1(void * arg)
{
	tNumber = 1;
	printf("thread1 errno:%p, &gNumber:%p\n", &errno, &gNumber);
	printf("thread1 tNumber:%d, &tNumber:%p\n", tNumber, &tNumber);
	return nullptr;
}

void * threadFunc2(void * arg)
{
	tNumber = 2;
	printf("thread2 errno:%p, &gNumber:%p\n", &errno, &gNumber);
	printf("thread2 tNumber:%d, &tNumber:%p\n", tNumber, &tNumber);

	return nullptr;
}
 
int main(void)
{
	printf("main thread errno:%p, &gNumber:%p\n", &errno, &gNumber);
	printf("main thread tNumber:%d, &tNumber:%p\n", tNumber, &tNumber);
	pthread_t pthid1, pthid2;
	pthread_create(&pthid1, nullptr, threadFunc1, nullptr);
	pthread_create(&pthid2, nullptr, threadFunc2, nullptr);

	pthread_join(pthid1, nullptr);
	pthread_join(pthid2, nullptr);

	return 0;
}
