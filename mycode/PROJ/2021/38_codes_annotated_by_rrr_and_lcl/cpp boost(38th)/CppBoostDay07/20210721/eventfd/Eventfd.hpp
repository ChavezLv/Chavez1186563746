 ///
 /// @file    Eventfd.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 10:02:37
 ///
 
#ifndef __WD_EVETNFD_HPP__
#define __WD_EVETNFD_HPP__

#include <functional>

/*
 $man eventfd
read：如果计数器A的值不为0时，读取成功，获得到该值。
如果A的值为0，非阻塞模式时，会直接返回失败，并把error置为EINVAL;
如果为阻塞模式，一直会阻塞到A为非0为止。

write：将缓冲区写入的8字节整形值加到内核计数器上，
即会增加8字节的整数在计数器A上，如果其值达到0xfffffffffffffffe时，
就会阻塞（在阻塞模式下），直到A的值被read。

select/poll/epoll：支持被io多路复用监听。
当内核计数器的值发生变化时，就会触发事件
*/
namespace wd
{

using EventfdCallback = std::function<void()>;

class Eventfd
{
public:
	Eventfd(EventfdCallback && cb);

	void start();
	void stop();

	void wakeup();
private:
	int createEventfd();
	void handleRead();

private:
	int _eventfd;
	EventfdCallback _cb;
	bool _isStarted;//是否开始进行监听

};

}//end of namespace wd
 
 
 
#endif
