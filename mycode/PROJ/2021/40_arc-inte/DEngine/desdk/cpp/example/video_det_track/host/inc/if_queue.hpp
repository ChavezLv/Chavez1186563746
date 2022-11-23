/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file     if_queue.hpp
/// @brief    进程间队列头文件
/// @author   caoyan
/// @date     2019/8/21
/// @version  0.0.1
/// @par      Copyright(C) 2018, Shenzhen Intellifusion Technologies Co.Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef IF_QUEUE_HPP
#define IF_QUEUE_HPP

#include "if_common.hpp"
#include <vector>
#include <queue>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class IfQueue
{
 public:
  IfQueue() {}
  ~IfQueue() {clear();}
  
  void clear() {       
    std::unique_lock<std::mutex> locker(lock_);
    queue_.clear();    
  }
  
  int size() const {
    std::unique_lock<std::mutex> locker(lock_);
    return queue_.size();   
  }
  
  T & front() const {
    std::unique_lock<std::mutex> locker(lock_);
    return queue_.front();
  }
  
  T & back() const {
    std::unique_lock<std::mutex> locker(lock_);
    return queue_.back();
  }
  
  T read() {           
    std::unique_lock<std::mutex> locker(lock_);
    if (queue_.empty()) {          
        cv_.wait(locker);     
    }
    T t = queue_.front();
    queue_.pop_front();
    
    return t;
  }
  
  void write(T val)
  {     
    std::unique_lock<std::mutex> locker(lock_);  
    queue_.emplace_back(val);
    cv_.notify_one();
  }
  
  typename std::list<T>::iterator begin() {return queue_.begin();}
  typename std::list<T>::iterator end() {return queue_.end();}
  typename std::list<T>::const_iterator begin() const {return queue_.begin();}
  typename std::list<T>::const_iterator end() const {return queue_.end();}
  
 protected:
  std::list<T> queue_;
  std::mutex lock_;
  std::condition_variable cv_;
  DISALLOW_COPY_AND_ASSIGN(IfQueue);
};


#endif //IF_QUEUE_HPP

