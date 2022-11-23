 ///
 /// @file    Producer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:41:20
 ///
 
#ifndef __WD_PRODUCER_HPP__
#define __WD_PRODUCER_HPP__

#include "Thread.hpp"
#include "TaskQueue.hpp"

#include <unistd.h>
#include <stdlib.h>

#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

class Producer
: public Thread
{
public:
	Producer(TaskQueue & taskque)
	: _taskque(taskque)
	{}

	void run()
	{
		int cnt = 20;
		srand(time(nullptr));
		while(cnt-- > 0) {
			int number = rand() % 100;
			cout << ">> producer make a number " << number << endl;
			_taskque.push(number);
			/* sleep(1); */
		}
	}

private:
	TaskQueue & _taskque;
};

}// end of namespace wd
 
 
 
#endif
