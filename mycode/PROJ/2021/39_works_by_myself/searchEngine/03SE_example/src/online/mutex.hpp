#ifndef __RXL_MUTEX_H__
#define __RXL_MUTEX_H__
#include"noncopy.hpp"
#include<unistd.h>
#include<pthread.h>
#include<iostream>
using std::cout;
using std::endl;
namespace rxl{

class Mutex
:Noncopy
{
public:
	Mutex()
	:_isLocking(false)
	{
		if(0 != pthread_mutex_init(&_mutex,NULL)){
			perror("pthread_mutex_init");
		}
	}
	~Mutex(){
		int ret = pthread_mutex_destroy(&_mutex);
		if(ret){
			cout<<"ret = "<<ret<<endl;
			perror("pthread_mutex_destroy");
		}
	}
	void lock(){
		if(0 != pthread_mutex_lock(&_mutex)){
			perror("pthread_mutex_lock");
			return;
		}
		_isLocking = true;
	}
	void unlock(){
		if(0 != pthread_mutex_unlock(&_mutex)){
			perror("pthread_mutex_unlock");
			return;
		}
		_isLocking = false;
	}
	pthread_mutex_t* getMutex(){
		return &_mutex;
	}
	bool isLocking() const{
		return _isLocking;
	}
private:
	pthread_mutex_t _mutex;
	bool _isLocking;
};
class MutexGuard{
public:
	MutexGuard(Mutex &mutex)
	:_mutexguard(mutex)
	{
		_mutexguard.lock();
	}
	~MutexGuard(){
		_mutexguard.unlock();
	}
private:
	Mutex& _mutexguard;
};

}//end of namespace
#endif
