 ///
 /// @file    Producer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:41:20
 ///
 
#ifndef __WD_PRODUCER_HPP__
#define __WD_PRODUCER_HPP__

#include "TaskQueue.hpp"

#include <unistd.h>
#include <stdlib.h>

#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

//Producer是一个单独的具体类, 没有继承关系
class Producer
{
public:
	void produce(TaskQueue & taskque)
	{
		int cnt = 20;
		srand(time(nullptr));
		while(cnt-- > 0) {
			int number = rand() % 100;
			cout << ">> producer make a number " << number << endl;
			taskque.push(number);
			/* sleep(1); */
		}
	}
};

}// end of namespace wd
 
 
 
#endif
