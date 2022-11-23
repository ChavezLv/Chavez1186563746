//
// Created by Admin on 2021/9/16.
//

#ifndef ARCTERN_BASE_SRC_COMMON_THREADSAFEMAP_H_
#define ARCTERN_BASE_SRC_COMMON_THREADSAFEMAP_H_
#include "src/common/ReadWriteLock.h"
#include <map>
///> only finished have be used api
template<typename KeyType,typename ValueType>
class ThreadSafeMap{
 public:
  ThreadSafeMap():m_map(){};
  virtual ~ThreadSafeMap() =default;
  ThreadSafeMap(const ThreadSafeMap<KeyType,ValueType>& safe_map){
    if(&safe_map == this) return;
    WriteLock_Guard lock_guard(&lock_);
    m_map = safe_map.m_map;
  }
  ThreadSafeMap &  operator=(const ThreadSafeMap<KeyType,ValueType>& safe_map){
    if(&safe_map == this) return *this;
    WriteLock_Guard lock_guard(&lock_);
    m_map = safe_map.m_map;
    return *this;
  }
  ThreadSafeMap( ThreadSafeMap<KeyType,ValueType>&& safe_map) = delete;
  ThreadSafeMap &  operator=( ThreadSafeMap<KeyType,ValueType>&& safe_map) = delete;

  ValueType& operator[](const KeyType& key){
    WriteLock_Guard lock_guard(&lock_);
    return m_map[key];
  }

  bool find(const KeyType & key){
    ReadLock_Guard lock_guard(&lock_);
    if(m_map.find(key) != m_map.end()){
      return true;
    }
    else return false;
  }

 private:
  std::map<KeyType,ValueType> m_map;
  ReadWriteLock lock_;
};

#endif //ARCTERN_BASE_SRC_COMMON_THREADSAFEMAP_H_
