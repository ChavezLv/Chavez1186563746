/*================================================================
*   
*   @file       ：TaskQueue.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "TaskQueue.hpp"

namespace wd
{

TaskQueue::TaskQueue(size_t capacity)
: _que()
, _capacity(capacity)
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{}

bool TaskQueue::empty() const
{	return _que.size() == 0;	}

bool TaskQueue::full() const
{	return _que.size() == _capacity;	}

//运行在生产者线程
void TaskQueue::push(const ElemType & e)
{
	//1.利用RAII技术，当局部对象创建时加锁；
	//当局部对象销毁解锁
	{
	MutexLockGuard autolock(_mutex);
	while(full()) {//2.防止虚假唤醒的情况出现，必须要使用while，而不能使用if
		_notFull.wait();//异常（虚假）唤醒,面试考点
	}
	//...假设代码在这里，由其他人维护时，
	//导致函数直接退出了
	//return;

	_que.push(e);//放入一个数据之后，
	}
	_notEmpty.notify();//要通知消费者线程取数据
}

void TaskQueue::push(ElemType && e)
{
	{
	MutexLockGuard autolock(_mutex);
	while(full()) {
		_notFull.wait();
	}

	_que.push(std::move(e));
	}
	_notEmpty.notify();
}
//运行在消费者线程
ElemType TaskQueue::pop()
{
	//用语句块来控制局部对象的生命周期
	//进而控制加锁的范围
	ElemType tmp;
	MutexLockGuard autolock(_mutex);
	while(_flag && empty()) {
		_notEmpty.wait();
	}

	if(_flag) {
		tmp = _que.front();
		_que.pop();
	
		_notFull.notify();
		return tmp;
	} else {
		return nullptr;
	}
}

void TaskQueue::wakeup()
{
	_flag = false;
	_notEmpty.notifyAll();
}


}//end of namespace wd
