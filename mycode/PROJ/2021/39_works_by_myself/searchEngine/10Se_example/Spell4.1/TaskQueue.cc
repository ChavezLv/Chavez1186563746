 ///
 /// @file    TaskQueue.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-05-06 12:01:04
 ///
 
#include "TaskQueue.hpp"
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

bool TaskQueue::empty()
{
	/* MutexLockGuard mlg(_mutex); */
	return _que.empty();
}

bool TaskQueue::full() 
{
	/* MutexLockGuard mlg(_mutex); */
	return _que.size() == _queSize;
}

//生产者线程会调用的方法
void TaskQueue::push(Elemtype data)
{
	//队列之中是否还有位置存放数据
	//_mutex.lock();
	MutexLockGuard protec(_mutex);
	while(full()) {
		_cond.wait();//阻塞
	}

	//队列之中还有位置
	_que.push(data);

	//通知消费者线程取数据
	_cond.notify();
}

//消费者线程需要做的事情
Elemtype TaskQueue::pop()
{
	//队列之中是否有数据
	MutexLockGuard protec(_mutex);
	while(_flags&&empty()) {
		_cond.wait();//阻塞
    }
    if(_flags){
        Elemtype data = _que.front();
        _que.pop();

        //通知生产者线程放入数据
        _cond.notify();
        return data;

    }
    return nullptr;
}

void TaskQueue::wakeUpAll(){
    if(_flags){
        _flags=false;
    }
    _cond.notifyAll();
}
}//end of namespace wd
