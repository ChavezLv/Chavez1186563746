#include"thread.hpp"
namespace rxl{
namespace currentThread{
	__thread const char* threadName = "rxl thread";
}//end of namespace currentThread

struct ThreadData{
string _name;
ThreadCallBack _cb;

ThreadData(const string& name,const ThreadCallBack& cb)
:_name(name)
,_cb(cb)
{}

void runInThread(){
	currentThread::threadName = (_name == string()? "rxl thread":_name.c_str() );
	if(_cb)
		_cb();
}
};

Thread::~Thread(){
	if(_isRunning){
		pthread_detach(_tid);		
	}
}

void Thread::create(){
	ThreadData* pdata = new ThreadData(_name,_cb);
	if(0 != pthread_create(&_tid,nullptr,Run,pdata)){
		perror("pthread_create");
		return;
	}
	_isRunning = true;
}

void Thread::join(){
	if(_isRunning)
	{
		pthread_join(_tid,nullptr);
		_isRunning = false;
	}
}

void* Thread::Run(void* args){
	ThreadData* pdata = static_cast<ThreadData*>(args);
	if(pdata){
		pdata->runInThread();
		return (void*)0;
	}
	return nullptr;
}

}//end of namespace
