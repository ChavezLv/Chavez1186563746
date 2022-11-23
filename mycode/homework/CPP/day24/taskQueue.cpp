#include"taskQueue.h"
TaskQueue::TaskQueue(size_t quesize)
    :_queSize(quesize)
     ,_que(),_mutex()
     ,_notFull(_mutex)
     ,_notEmpty(_mutex){}
TaskQueue::~TaskQueue(){}
bool TaskQueue::empty()const{
    return 0==_que.size();
}
bool TaskQueue::full()const{
    return _que.size()==_queSize;
}

void TaskQueue::push(const int &value){
    while(full()){
        _notFull.wait();
    }
    _que.push(value);
}
int TaskQueue::pop(){
    
}
