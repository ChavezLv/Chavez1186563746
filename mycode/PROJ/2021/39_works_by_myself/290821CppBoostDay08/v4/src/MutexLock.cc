 ///
 /// @file    MutexLock.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-23 17:31:44
 ///
 
#include "MutexLock.hpp"

namespace wd
{

MutexLock::MutexLock()
{
	pthread_mutex_init(&_mutex, nullptr);
}

MutexLock::~MutexLock()
{
	pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock()
{
	pthread_mutex_lock(&_mutex);
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&_mutex);
}

}//end of namespace wd

