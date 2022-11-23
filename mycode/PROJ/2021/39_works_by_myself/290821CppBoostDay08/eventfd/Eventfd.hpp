 ///
 /// @file    Eventfd.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:02:49
 ///
 
#ifndef __WD_EVENTFD_HPP__
#define __WD_EVENTFD_HPP__

#include <functional>

using std::function;

namespace wd
{

class Eventfd
{
using EventCallback = function<void()>;
public:
	Eventfd(EventCallback && cb);
	~Eventfd();

	void start();
	void stop();
	void wakeup();
private:
	int createEventfd();
	void handleRead();

private:
	int _eventfd;
	EventCallback _cb;
	bool _isStarted;
};

}//end of namespace 
 
 
 
#endif
