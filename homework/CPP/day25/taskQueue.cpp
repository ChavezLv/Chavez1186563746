#include"taskQueue.h"
TaskQueue::TaskQueue(size_t queSize)
    :_que(),
    _queSize(queSize),
    _mutex(),
    _full(_mutex),
    _empty(_mutex),
    _flag(true){}
TaskQueue::~TaskQueue(){}
void TaskQueue::push(ElemType value){
    /*可以运用RAII的思想*/
    _mutex.lock();
    while(isFull()){
        _full.wait();
    }
    _que.push(value);
    _full.notify();
    _mutex.unlock();
}
ElemType TaskQueue::pop(){
    _mutex.lock();
    while(_flag&&isEmpty()){
        _empty.wait();
    }
    if(_flag){
        ElemType tmp=_que.front();
        _que.pop();
        /*_notEmpty*/
        _empty.notify();
        return tmp;
    }
    _mutex.unlock();
    return nullptr;
}
bool TaskQueue::isFull(){
    return _que.size()==_queSize;
}
bool TaskQueue::isEmpty(){
    return _que.empty();
}
/*一旦调用,便阻止线程从任务队列中获取任务
 * 所有线程都将处于非阻塞状态,然后进行线程回收*/
void TaskQueue::wakeup(){
    _flag=false;
    _empty.notifyAll();
}
