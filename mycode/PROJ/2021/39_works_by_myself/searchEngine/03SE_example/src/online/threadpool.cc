#include"threadpool.hpp"
#include<unistd.h>
namespace rxl{
void Threadpool::addTask(CallBackFunc&&  cb){	
	_taskQue.push(cb);
}
CallBackFunc Threadpool::getTask(){
	return _taskQue.pop();
}
void Threadpool::start(){
	for(int i = 0 ;i<_threadNum;++i){
		unique_ptr<Thread> up(new Thread(std::bind(&Threadpool::threadFunc,this),std::to_string(i)));
		_threads.push_back(std::move(up));
	}
	for(auto& thread : _threads){
		thread->create();
	}
}
void Threadpool::stop(){
	if(!_isExit){
		while(!_taskQue.empty()){
			sleep(1);
		}
		_isExit = true;
		_taskQue.weakup();
		for(auto &thread : _threads){
			thread->join();
		}
	}
}
void Threadpool::threadFunc(){
	while(!_isExit){
		CallBackFunc cb = getTask();
		if(cb){
			cb();
		}
	}
}

}//end of namespace
