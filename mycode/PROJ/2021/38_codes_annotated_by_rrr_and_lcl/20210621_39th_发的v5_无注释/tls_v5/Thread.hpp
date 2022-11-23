 ///
 /// @file    Thread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-11 16:52:09
 ///
 
#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

#include <string>
#include <functional>
using std::string;
 
namespace wd
{


/* __thread int idx = 0; */

namespace current_thread 
{
extern __thread const char * name;
}

class Thread : Noncopyable
{
public:
	using ThreadCallback = std::function<void()>;
	Thread(ThreadCallback && cb, const string & name = "wd thread");
	~Thread();
	void start();
	void join();

private:
	static void * threadFunc(void *);

private:
	pthread_t _pthid;
	string _name;
	bool _isRunning;
	ThreadCallback _cb;
};


}//end of namespace wd
 
 
#endif
