#ifndef __RXL_TASKQUEUE_H__
#define __RXL_TASKQUEUE_H__
#include"mutex.hpp"
#include"cond.hpp"
#include<functional>
#include<queue>
#include<iostream>
using std::endl;
using std::cout;
using std::queue;
namespace rxl{
class TaskQueue{
using ElemType = std::function<void()>;
public:
	TaskQueue(size_t size = 10)
	:_size(size)
	,_mutex()
	,_cond(_mutex)
	,_AbleWaitFlag(true)
	{} 
	bool empty(){ return _que.empty();}
	bool full(){ return _size == _que.size();}
	void push(ElemType data){
		MutexGuard mutextGuard(_mutex);
		while(full()){
			_cond.wait();
		}
		_que.push(data);
		_cond.notify();
	}
	ElemType pop(){
		MutexGuard mutextGuard(_mutex);
		while(_AbleWaitFlag && empty()){
			_cond.wait();
		}
		if(_AbleWaitFlag){
			ElemType data = _que.front();
			_que.pop();
			_cond.notify();
			return data;
		}else {
			return nullptr;
		}
	}
	void weakup(){
		if(_AbleWaitFlag){
			_AbleWaitFlag = false;
		}
		_cond.notifyAll();
	}
private:
	queue<ElemType> _que;
	size_t _size;
	Mutex _mutex;
	Cond _cond;
	bool _AbleWaitFlag;
};

}//end of namespace
#endif
