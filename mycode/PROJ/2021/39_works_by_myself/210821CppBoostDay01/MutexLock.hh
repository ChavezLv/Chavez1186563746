#ifndef __WD_MUTEXLOCK_HH__
#define __WD_MUTEXLOCK_HH__

/* #include <stdio.h>// perror */
#include <string.h>
#include <pthread.h>
#include <iostream>

/*封装互斥锁的时候，要用到的方法，*/
//问题一：MutexLock和Condition是否要设计成单例模式？
//     单例模式只能通过该类创建出一个对象，这意味着只能创建一把锁，如果再来一把锁的话，
//      这显然不符合需求，锁可以有多把，条件变量也可以有多个
 
//问题二：MutexLock和Condition的成员是否要设计成static?
//      不能设计成static,否则只能是唯一的
 
//问题三：MutexLock与Condition应该是什么关系？
//       从互斥锁和条件变量的创建以及销毁来看，彼此并不负责对方的生命周期，互不相干，彼此独立
//          由于Condition的接口wait方法是无参的，但是接下来的操作中需要用到互斥锁，MuteLock
//        只能以指针的形式出现在Condition的数据成员中，表达的是一种关联关系（A has B)
 
 
//问题四：成员变量引用，需要注意构造函数初始化列表中进行
using std::cout;
using std::endl;

/* printf("%s:%s-%d\n",funcName,strerror(ret),__LINE__);\ */
#define THREAD_ERROR_CHECK(ret, funcName) do{\
    if(0 != ret){\
        cout << "line "<< __LINE__ << ": pthread_mutex_init " << strerror(ret) << endl;\
    }\
}while(0)

namespace wd{

class MutexLock
{
public: 
    MutexLock()
    {
        pthread_mutex_init(&_mutex, nullptr);
        THREAD_ERROR_CHECK(pthread_mutex_init(&_mutex, nullptr), pthread_mutex_init);

    }

    MutexLock(const MutexLock &) = delete;//拷贝构造
    MutexLock & operator=(const MutexLock &) = delete;//赋值运算符

    ~MutexLock(){
        if(0 != pthread_mutex_destroy(&_mutex)){
            perror("pthread_mutex_destroy");
        }
    }

    void lock(){
        if(0 != pthread_mutex_lock(&_mutex)){
            perror("pthread_mutex_lock");
        }
    }

    void unlock(){
        if(0 != pthread_mutex_unlock(&_mutex)){
            perror("pthread_mutex_unlock");
        }
    }

    pthread_mutex_t& getMutex(){
        return _mutex;
    }
    
private:
    //...
    pthread_mutex_t _mutex;
};

class MutexLockGuard{//RAII + { } 利用C++的构造析构精准控制加锁解锁
public:
    explicit
        MutexLockGuard(MutexLock & mutex)
        : _mutex(mutex){
            _mutex.lock();//构造函数加锁
        }

    ~MutexLockGuard(){
        _mutex.unlock();//析构函数来解锁
    }
private:
    MutexLock &_mutex;
};

}//end of namespace wd

#endif
