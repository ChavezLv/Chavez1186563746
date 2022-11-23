 ///
 /// @file    Consumer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:46:21
 ///
 
#ifndef __WD_CONSUMER_HPP__
#define __WD_CONSUMER_HPP__

#include "Thread.hpp"
#include "TaskQueue.hpp"

#include <unistd.h>
#include <stdlib.h>


#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

class Consumer 
: public Thread
{
public:
	Consumer(TaskQueue & taskque)
	: _taskque(taskque)
	{}

	void run()
	{
		int cnt = 20;
		while(cnt-- > 0) {
			int number = _taskque.pop();
			cout << ">> consumer get a number " << number << endl;
			sleep(1);
		}
	}

private:
	TaskQueue & _taskque;
};

}//end of namespace wd
 
 
 
#endif
