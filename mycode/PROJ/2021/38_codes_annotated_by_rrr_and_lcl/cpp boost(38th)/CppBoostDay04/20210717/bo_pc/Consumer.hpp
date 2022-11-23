 ///
 /// @file    Consumer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:46:21
 ///
 
#ifndef __WD_CONSUMER_HPP__
#define __WD_CONSUMER_HPP__

#include "TaskQueue.hpp"

#include <unistd.h>
#include <stdlib.h>


#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

//Consumer是一个单独的具体类, 没有继承关系
class Consumer 
{
public:
	Consumer()
	{}

	void consume(TaskQueue & taskque)
	{
		int cnt = 20;
		while(cnt-- > 0) {
			int number = taskque.pop();
			cout << ">> consumer get a number " << number << endl;
			sleep(1);
		}
	}
};

}//end of namespace wd
 
 
 
#endif
