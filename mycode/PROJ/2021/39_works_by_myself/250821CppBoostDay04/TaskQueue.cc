/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：TaskQueue.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 22:03:13
*   @description：
*
================================================================*/


#include "TaskQueue.hh"

namespace wd
{

/* template<typename T> */
/* TaskQueue<T>::TaskQueue(size_t capacity) */
/* : _que() */
/* , _capacity(capacity) */
/* , _mutex() */
/* , _notFull(_mutex) */
/* , _notEmpty(_mutex) */
/* , _isWorking(true){ */

/* } */

/* template<typename T> */
/* bool TaskQueue<T>::empty() const{ */
/*     return  0 == _que.size(); */
/* } */

/* template<typename T> */
/* bool TaskQueue<T>::full() const{ */
/*     return _capacity == _que.size(); */
/* } */

/* /1* void TaskQueue::push(int number){ *1/ */
/* template<typename T> */
/* void TaskQueue<T>::push(const ElemType &elem){ */
/*     { //类 MutexLockGuard + 语句块 {  },  注意这里与直接调用析构函数的区别, 对象生命周期结束则析构函数必然会被调用, 但是调用析构函数时对象不一定会销毁. */
/*         MutexLockGuard mutexGuard(_mutex); //加锁 */
/*         while (full()) */
/*         {                    //虚假唤醒, 正常情况下signal只会唤醒一个线程, 但是比如很多生产者都在等的时候, 所有的生产者在遇到signal时都会被唤醒. */
/*             _notFull.wait(); //满的 */
/*         } */

/*         _que.push(elem); //不满时, 退出while */
/*     }                      //解锁 */

/*     _notEmpty.notify();//通知消费者 */

/* } */

/* template<typename T> */
/* typename TaskQueue<T>::ElemType TaskQueue<T>::pop(){ */
/* /1* ElemType TaskQueue<T>::pop(){ *1/ */
/*     MutexLockGuard mutexGuard(_mutex); */
/*     while(empty() && _isWorking){ */
/*         _notEmpty.wait(); */
/*     } */
/*     if(!_isWorking){//当队列为空 */
/*         return 0; */
/*     } */

/*     ElemType popElem = _que.front(); */
/*     _que.pop(); */
/*     _notFull.notify();//通知生产者 */

/*     return std::move(popElem); */
/* } */

}// end of namespace wd
