#ifndef __RXL_COND_H__
#define __RXL_COND_H__
#include "noncopy.hpp"
#include "mutex.hpp"
#include<pthread.h>
#include<iostream>
using std::cout;
using std::endl;
namespace rxl{
class Cond
:Noncopy
{
	public:
		Cond(Mutex &mutex)
		:_cond()
		,_Mutex(mutex)
	    {
		if( 0 != pthread_cond_init(&_cond,NULL)){
			perror("pthread_cond_init");
		}
	    }
		~Cond(){
			if(0 != pthread_cond_destroy(&_cond)){
				perror("pthread_cond_destory");
			}
		}
		void wait(){
			if(0 != pthread_cond_wait(&_cond,_Mutex.getMutex())){
				perror("pthread_cond_wait");
			}
		}
		void notify(){
			if(0 != pthread_cond_signal(&_cond)){
				perror("pthread_cond_signal");
			}
		}
		void notifyAll(){
			if(0 != pthread_cond_broadcast(&_cond)){
				perror("pthread_cond_broadcast");
			}
		}
	private:
		pthread_cond_t _cond;
		Mutex& _Mutex;
};
}//end of namespace
#endif
